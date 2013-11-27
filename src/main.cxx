#include <iostream>
#include <iomanip>

#include "rbtree.hxx"
#include "tree_traversing.hxx"

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

#define TRAVERSE(Strategy, tree, functor)                 \
  {                                                       \
    std::cout << "Traverse using "#Strategy << std::endl; \
    traverse_by<Strategy>(((tree)), ((functor)));         \
    std::cout << std::endl;                               \
  }

int main(int argc, char *argv[]) {
  RBTree<int> tree;

  for (int i = 1; i <= 32; ++i) {
    tree.insert(i);
  }

  std::cout << std::endl;
  std::cout << "Traversing:" << std::endl;

  TRAVERSE(TreeTraverseRecursivePLR, tree,
      [](RBTree<int>::Node const *node) {
        std::cout << node->value() << " ";
      });
  std::cout << std::endl;

  TRAVERSE(TreeTraverseIterativePLR, tree,
      [](RBTree<int>::Node const *node) {
        std::cout << node->value() << " ";
      });
  std::cout << std::endl;

  TRAVERSE(TreeTraverseRecursiveLPR, tree,
      [](RBTree<int>::Node const *node) {
        std::cout << node->value() << " ";
      });
  std::cout << std::endl;

  TRAVERSE(TreeTraverseIterativeLPR, tree,
      [](RBTree<int>::Node const *node) {
        std::cout << node->value() << " ";
      });
  std::cout << std::endl;

  TRAVERSE(TreeTraverseRecursiveLRP, tree,
      [](RBTree<int>::Node const *node) {
        std::cout << node->value() << " ";
      });
  std::cout << std::endl;

  TRAVERSE(TreeTraverseIterativeLRP, tree,
      [](RBTree<int>::Node const *node) {
        std::cout << node->value() << " ";
      });
  std::cout << std::endl;

  TRAVERSE(TreeTraverseIterativeHorizontally, tree,
      [](RBTree<int>::Node const *node) {
        std::cout << node->value() << " ";
      });
  std::cout << std::endl;

  unsigned int plevel = -1;
  TRAVERSE(TreeTraverseIterativeHorizontallyWithLevel, tree,
      [&plevel](RBTree<int>::Node const *node, unsigned int level) {
        if (plevel != level) {
          std::cout << std::endl << std::setw(5) << level << ": ";
          plevel = level;
        } else
          std::cout << " ";
        std::cout << node->value();
      });
  std::cout << std::endl;

  dump_to_dot(tree, std::cerr);

  return 0;
}