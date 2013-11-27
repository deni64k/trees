#include <iostream>
#include <fstream>

#include "rbtree.hxx"

template <class Tree>
static void dump_to_dot(Tree const &tree, std::ostream &out) {
	out << "digraph {\n";

	tree.traverse([&out](typename Tree::Node const *n) {
		std::string const color = n->color() == Tree::Node::Color::Red ? "red" : "black";
		out << "n" << n << " [label=" << n->value() << ", color=" << color << ", fontcolor=white, style=filled];\n";

		if (auto const *l = n->left())
			out << "n" << n << " -> n" << l << " [side=left];\n";

		if (auto const *r = n->right())
			out << "n" << n << " -> n" << r << " [side=right];\n";

		out.flush();
	});

	out << "}\n";
}

int main(int argc, char *argv[]) {
	RBTree<int> tree;

	// for (int i = 1; i <= 8; ++i) {
	// 	tree.insert(i);
	// }

	// for (int i = 10; i <= 159; ++i) {
	// 	tree.insert(i);
	// }

	// for (int i = 320; i >= 160; --i) {
	// 	tree.insert(i);
	// }

	// for (int i = 0; i <= 9 ; ++i) {
	// 	tree.insert(i);
	// }
	// for (int i = 0; i <= 9 ; ++i) {
	// 	tree.insert(i);
	// }

	for (int i = 1; i <= 512; ++i) {
		tree.insert(i);
	}

	std::cout << "Find 11: " << tree.find(11)->value() << std::endl;
	std::cout << "Find 1100: " << tree.find(1100) << std::endl;

	std::cout << "Maximum: " << tree.maximum()->value() << std::endl;
	std::cout << "Minimum: " << tree.minimum()->value() << std::endl;

	// tree.insert(60);
	// tree.insert(61);
	// tree.insert(62);
	// tree.insert(63);

	// for (int i = 1; i <= 6; ++i) {
	// 	tree.insert(i);
	// }

	if (argc > 1) {
		std::ofstream os(argv[1]);

		dump_to_dot(tree, os);
	}

	return 0;
}