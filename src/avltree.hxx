#pragma once

#include <iostream>
#include <algorithm>

template <class Value>
struct AVLTree {
	struct Node {
		typedef int Height;

		Node() = default;
		Node(Value const &value)
		: value_(value)
		{}
		Node(Value &&value)
		: value_(value)
		{}

		Node const * parent() const { return parent_; }
		Node const * left()   const { return left_;   }
		Node const * right()  const { return right_;  }
		Node       * parent()       { return parent_; }
		Node       * left()         { return left_;   }
		Node       * right()        { return right_;  }
		Height const & height() const { return height_; }
		Value  const & value()  const { return value_; }

		void set_parent(Node *n) { parent_ = n; }
		void set_left(Node *n)   { left_   = n; }
		void set_right(Node *n)  { right_  = n; }

		void set_height(Height const &height)  { height_  = height; }

		Node * grandparent() const {
			return parent_ ? parent_->parent() : nullptr;
		}
		Node * uncle() const {
			auto const g = grandparent();
			if (g)
				return parent_ == g->left() ? g->right() : g->left();
			else
				return nullptr;
		}

		Height yield_height() const {
			auto const hl = left_  ? left_->yield_height()  + 1 : 0;
			auto const hr = right_ ? right_->yield_height() + 1 : 0;
			return std::max(hr, hl);
		}

		int balance() const {
			auto const hl = left_  ? left_->yield_height()  : 0;
			auto const hr = right_ ? right_->yield_height() : 0;
			return hr - hl;
		}

	private:
		Node   *parent_ = nullptr;
		Node   *left_   = nullptr;
		Node   *right_  = nullptr;
		Value   value_;
		Height  height_;
	};

	AVLTree() = default;

	Node const * insert(Value const &value) {
		std::cout << "Insert " << value << "\n";
		Node *node = new Node(value);
		insert_to_bst_(node);

		if (node->parent())
			incr_heights_(node->parent());

		fixup_(node);

		return node;
	}

	template <class Functor>
	void traverse(Functor const &functor) const {
		if (!root_)
			return;

		functor(root_);

		traverse_recursive_(functor, root_);
	}

protected:
	void rotate_left_(Node *node) {
		auto *c = node->right();
		auto *p = node->parent();
		std::cout << __FUNCTION__ << ": node " << node->value() << ", c " << (c ? c->value() : -1) << ", p " << (p ? p->value() : -1) << "\n";

		if (p) {
			if (p->right() == node)
				p->set_right(c);
			else
				p->set_left(c);
		}
		node->set_right(c->left());
		if (c->left())
			c->left()->set_parent(node);
		c->set_parent(p);
		c->set_left(node);
		node->set_parent(c);

		if (root_ == node)
			root_ = c;
	}

	void rotate_right_(Node *node) {
		auto *c = node->left();
		auto *p = node->parent();
		std::cout << __FUNCTION__ << ": node " << node->value() << ", c " << (c ? c->value() : -1) << ", p " << (p ? p->value() : -1) << "\n";

		if (p) {
			if (p->right() == node)
				p->set_right(c);
			else
				p->set_left(c);
		}
		node->set_left(c->right());
		if (c->right())
			c->right()->set_parent(node);
		c->set_parent(p);
		c->set_right(node);
		node->set_parent(c);

		if (root_ == node)
			root_ = c;
	}

	void insert_to_bst_(Node *new_node) {
		Node *prev_node = nullptr;
		Node *node      = root_;

		if (!root_) {
			root_ = new_node;
			return;
		}

		while (true) {
			prev_node = node;
			if (new_node->value() < node->value()) {
				if (!(node = node->left())) {
					prev_node->set_left(new_node);
					new_node->set_parent(prev_node);
					break;
				}
			} else {
				if (!(node = node->right())) {
					prev_node->set_right(new_node);
					new_node->set_parent(prev_node);
					break;
				}
			}
		}

		return;
	}

	void incr_heights_(Node *node) {
		if (node) {
			auto const *l = node->left();
			auto const *r = node->right();

			auto lh = l ? l->height() : 0;
			auto rh = r ? r->height() : 0;

			node->set_height(std::max(lh, rh) + 1);
		}

		if (auto *p = node->parent())
			incr_heights_(p);
	}

	void fixup_(Node *node) {
		Node *n = node;

		while (n) {
			auto const balance = n->balance();
			std::cout << __FUNCTION__ << " node " << n->value() << " " << balance << std::endl;

			if (balance > 1) {
				Node *r = n->right();
				if (r->balance() < 0) {
					rotate_right_(r);
					fixup_(r->parent());
				}
				rotate_left_(n);
			}	else
			if (balance < -1) {
				Node *l = n->left();
				if (l->balance() > 0) {
					rotate_left_(l);
					fixup_(l->parent());
				}
				rotate_right_(n);
			}

			n = n->parent();
			if (n)
				std::cout << __FUNCTION__ << " next node " << n->value() << " " << n->balance() << std::endl;
		}
	}

private:
	template <class Functor>
	void traverse_recursive_(Functor const &functor, Node const *node) const {
		if (Node const *l = node->left()) {
			functor(l);
			traverse_recursive_(functor, l);
		}
		if (Node const *r = node->right()) {
			functor(r);
			traverse_recursive_(functor, r);
		}
	}

	Node *root_ = nullptr;
};
