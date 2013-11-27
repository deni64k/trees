#pragma once

#include <iostream>

template <class Value>
struct TreeNode {
	enum struct Color {
		Red, Black
	};

	enum struct Position {
		Root, Left, Right
	};

	TreeNode() = default;
	TreeNode(Value const &value)
	: value_(value)
	{}
	TreeNode(Value &&value)
	: value_(value)
	{}

	TreeNode const * parent() const { return parent_; }
	TreeNode const * left()   const { return left_;   }
	TreeNode const * right()  const { return right_;  }
	TreeNode       * parent()       { return parent_; }
	TreeNode       * left()         { return left_;   }
	TreeNode       * right()        { return right_;  }
	Value const & value() const { return value_; }
	Color const & color() const { return color_; }

	void set_color(Color const color) { color_ = color; }

	void set_parent(TreeNode *n) { parent_ = n; }
	void set_left(TreeNode *n)   { left_   = n; }
	void set_right(TreeNode *n)  { right_  = n; }

	TreeNode * grandparent() const {
		return parent_ ? parent_->parent() : nullptr;
	}
	TreeNode * uncle() const {
		auto const g = grandparent();
		if (g)
			return parent_ == g->left() ? g->right() : g->left();
		else
			return nullptr;
	}

	Position position() const {
		if (!parent_)
			return Position::Root;
		if (parent_->left() == this)
			return Position::Left;
		if (parent_->right() == this)
			return Position::Right;
	}

private:
	TreeNode *parent_ = nullptr;
	TreeNode *left_   = nullptr;
	TreeNode *right_  = nullptr;
	Color     color_  = Color::Red;
	Value     value_;
};
