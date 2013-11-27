#pragma once

template <class Value>
struct RBTree {
	struct Node {
		enum struct Color {
			Red = true, Black = false
		};

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
		Value const & value() const { return value_; }
		Color const & color() const { return color_; }

		void set_color(Color const color) { color_ = color; }

		void set_parent(Node *n) { parent_ = n; }
		void set_left(Node *n)   { left_   = n; }
		void set_right(Node *n)  { right_  = n; }

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

	private:
		Node  *parent_ = nullptr;
		Node  *left_   = nullptr;
		Node  *right_  = nullptr;
		Color  color_  = Color::Red;
		Value  value_;
	};

	RBTree() = default;

	Node const * insert(Value const &value) {
		// std::cout << "Insert " << value << "\n";
		Node *node = new Node(value);
		insert_to_bst_(node);

		fixup_(node);

		return node;
	}

	Node const * root() const {
		return root_;
	}

	Node const * find(Value const &value) {
		if (!root_)
			return nullptr;

		return find_in_subtree_(root_, value);
	}

	Node const * maximum() {
		if (!root_)
			return nullptr;

		Node *n = root_;
		while (n->right()) {
			n = n->right();
		}

		return n;
	}

	Node const * minimum() {
		if (!root_)
			return nullptr;

		Node *n = root_;
		while (n->left()) {
			n = n->left();
		}

		return n;
	}

	template <class Functor>
	void traverse(Functor const &functor) const {
		if (!root_)
			return;

		functor(root_);

		traverse_recursive_(functor, root_);
	}

	void fixup_(Node *node) {
		auto *p = node->parent();
		auto *g = node->grandparent();
		auto *u = node->uncle();

		// case 1:
		// Repaint root node to black.
		if (!p) {
			// std::cout << "Case 1: Value " << node->value() << "\n";
			node->set_color(Node::Color::Black);
			return;
		}

		// case 2:
		// Leave function if parent is black.
		if (p->color() == Node::Color::Black) {
			// std::cout << "Case 2: Value " << node->value() << "\n";
			return;
		}

		// case 3:
		// If both the parent and the uncle are red,
		// then both of them can be repainted black and
		// the grandpather becomes red.
		if ((p && p->color() == Node::Color::Red) &&
			  (u && u->color() == Node::Color::Red)) {
			// std::cout << "Case 3: Value " << node->value() << "\n";
			p->set_color(Node::Color::Black);
			u->set_color(Node::Color::Black);
			g->set_color(Node::Color::Red);

			fixup_(g);

			return;
		}

		// case 4:
		// The parent is red but uncle is black; the current node is the right child.
		if ((p && p->color() == Node::Color::Red) &&
			  (!u || (u && u->color() == Node::Color::Black))) {
			if (p->right() == node && g->left() == p) {
				// std::cout << "Case 4: Value " << p->value() << " rotate left\n";
				rotate_left_(p);

				node = p;
			} else if (p->left() == node && g->right() == p) {
				// std::cout << "Case 4: Value " << p->value() << " rotate right\n";
				rotate_right_(p);

				node = p;
			}
		}

		p = node->parent();
		g = node->grandparent();
		u = node->uncle();
		// case 5:
		// The parent is red but uncle is black; the current node is the left child.
		if ((p && p->color() == Node::Color::Red) &&
			  (!u || (u && u->color() == Node::Color::Black))) {
			// if (node->value() == 5)
			// 	return;
			p->set_color(Node::Color::Black);
			g->set_color(Node::Color::Red);

			if (p->left() == node) {
				// std::cout << "Case 5: Value " << g->value() << " rotate right\n";
				rotate_right_(g);
			} else if (p->right() == node) {
				// std::cout << "Case 5: Value " << g->value() << " rotate left\n";
				rotate_left_(g);
			}
		}
	}

protected:
	Node const * find_in_subtree_(Node const *node, Value const value) {
		if (!node)
			return nullptr;

	  if (value == node->value())
	  	return node;

		if (value < node->value()) {
			if (Node const *l = node->left())
		  	return find_in_subtree_(l, value);
	  } else {
			if (Node const *r = node->right())
		  	return find_in_subtree_(r, value);
		}

		return nullptr;
	}

	void rotate_left_(Node *node) {
		auto *c = node->right();
		auto *p = node->parent();
		// std::cout << __FUNCTION__ << ": node " << node->value() << ", c " << (c ? c->value() : -1) << ", p " << (p ? p->value() : -1) << "\n";

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
		// std::cout << __FUNCTION__ << ": node " << node->value() << ", c " << (c ? c->value() : -1) << ", p " << (p ? p->value() : -1) << "\n";

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
