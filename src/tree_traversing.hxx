#pragma once

#include <queue>
#include <stack>
#include <tuple>

template <typename Tree>
class TreeTraverseBase {
public:
  TreeTraverseBase(Tree const &tree)
  : tree_(tree)
  {}

protected:
  Tree const &tree_;
};

template <typename Tree>
class TreeTraverseRecursivePLR
: public TreeTraverseBase<Tree>
{
public:
  TreeTraverseRecursivePLR(Tree const &tree)
  : TreeTraverseBase<Tree>(tree)
  {}

  template <typename Functor>
  void traverse(Functor const &functor) const {
    typename Tree::Node const *root = TreeTraverseBase<Tree>::tree_.root();

    traverse_(functor, root);
  }

private:
  template <typename Functor>
  void traverse_(Functor const &functor, typename Tree::Node const *node) const {
    if (!node)
      return;

    functor(node);

    if (typename Tree::Node const *l = node->left()) {
      traverse_(functor, l);
    }
    if (typename Tree::Node const *r = node->right()) {
      traverse_(functor, r);
    }
  }
};

template <typename Tree>
class TreeTraverseRecursiveLPR
: public TreeTraverseBase<Tree>
{
public:
  TreeTraverseRecursiveLPR(Tree const &tree)
  : TreeTraverseBase<Tree>(tree)
  {}

  template <typename Functor>
  void traverse(Functor const &functor) const {
    typename Tree::Node const *root = TreeTraverseBase<Tree>::tree_.root();

    traverse_(functor, root);
  }

private:
  template <typename Functor>
  void traverse_(Functor const &functor, typename Tree::Node const *node) const {
    if (!node)
      return;

    if (typename Tree::Node const *l = node->left())
      traverse_(functor, l);

    functor(node);

    if (typename Tree::Node const *r = node->right())
      traverse_(functor, r);
  }
};

template <typename Tree>
class TreeTraverseRecursiveLRP
: public TreeTraverseBase<Tree>
{
public:
  TreeTraverseRecursiveLRP(Tree const &tree)
  : TreeTraverseBase<Tree>(tree)
  {}

  template <typename Functor>
  void traverse(Functor const &functor) const {
    typename Tree::Node const *root = TreeTraverseBase<Tree>::tree_.root();

    traverse_(functor, root);
  }

private:
  template <typename Functor>
  void traverse_(Functor const &functor, typename Tree::Node const *node) const {
    if (!node)
      return;

    if (typename Tree::Node const *l = node->left())
      traverse_(functor, l);

    if (typename Tree::Node const *r = node->right())
      traverse_(functor, r);

    functor(node);
  }
};

template <typename Tree>
class TreeTraverseIterativePLR
: public TreeTraverseBase<Tree>
{
public:
  TreeTraverseIterativePLR(Tree const &tree)
  : TreeTraverseBase<Tree>(tree)
  {}

  template <typename Functor>
  void traverse(Functor const &functor) const {
    typename Tree::Node const *root = TreeTraverseBase<Tree>::tree_.root();
    
    if (!root)
      return;

    std::stack<typename Tree::Node const *> retlist;
    retlist.push(root);
    
    do {
      typename Tree::Node const *node = retlist.top();
      retlist.pop();

      functor(node);
      
      if (typename Tree::Node const *r = node->right())
        retlist.push(r);
      if (typename Tree::Node const *l = node->left())
        retlist.push(l);
      
    } while (!retlist.empty());
  }
};

template <typename Tree>
class TreeTraverseIterativeLPR
: public TreeTraverseBase<Tree>
{
public:
  TreeTraverseIterativeLPR(Tree const &tree)
  : TreeTraverseBase<Tree>(tree)
  {}

  template <typename Functor>
  void traverse(Functor const &functor) const {
    typename Tree::Node const *root = TreeTraverseBase<Tree>::tree_.root();
    
    if (!root)
      return;

    std::stack<typename Tree::Node const *> retlist;
    retlist.push(root);

    typename Tree::Node const *l = retlist.top();
    while (l = l->left())
      retlist.push(l);
    
    do {
      typename Tree::Node const *node = retlist.top();
      retlist.pop();
      functor(node);

      if (typename Tree::Node const *r = node->right()) {
        retlist.push(r);
        typename Tree::Node const *l = r;
        while (l = l->left())
          retlist.push(l);
      }
    } while (!retlist.empty());
  }
};

template <typename Tree>
class TreeTraverseIterativeLRP
: public TreeTraverseBase<Tree>
{
public:
  TreeTraverseIterativeLRP(Tree const &tree)
  : TreeTraverseBase<Tree>(tree)
  {}

#define TRACEP { std::cerr << __FILE__ << " " << __FUNCTION__ << ":" << __LINE__ << std::endl; }
#define DUMPV(v) { std::cerr << __FILE__ << " " << __FUNCTION__ << ":" << __LINE__ << " " << #v << " //=> " << ((v)) << std::endl; }

  template <typename Functor>
  void traverse(Functor const &functor) const {
    typename Tree::Node const *root = TreeTraverseBase<Tree>::tree_.root();
    if (!root)
      return;

    typedef enum {LEFT, RIGHT, PARENT} QueueType;

    typedef std::tuple<typename Tree::Node const *, QueueType> RetlistPair;
    std::stack<RetlistPair> retlist;

    retlist.emplace(root, LEFT);

    while (!retlist.empty()) {
      typename Tree::Node const *node;
      QueueType queue_type;
      std::tie(node, queue_type) = retlist.top();
      
      switch (queue_type) {
      case LEFT:
        retlist.pop();
        retlist.emplace(node, RIGHT);
        if (typename Tree::Node const *n = node->left())
          retlist.emplace(n, LEFT);
        break;
      case RIGHT:
        retlist.pop();
        retlist.emplace(node, PARENT);
        if (typename Tree::Node const *n = node->right())
          retlist.emplace(n, LEFT);
        break;
      case PARENT:
        retlist.pop();
        functor(node);
        break;
      }
    }
  }
};

template <typename Tree>
class TreeTraverseIterativeHorizontally
: public TreeTraverseBase<Tree>
{
public:
  TreeTraverseIterativeHorizontally(Tree const &tree)
  : TreeTraverseBase<Tree>(tree)
  {}

  template <typename Functor>
  void traverse(Functor const &functor) const {
    typename Tree::Node const *node = TreeTraverseBase<Tree>::tree_.root();

    if (!node)
      return;

    std::queue<typename Tree::Node const *> retlist;
    retlist.push(node);

    do {
      typename Tree::Node const *node = retlist.front();
      retlist.pop();

      functor(node);

      if (typename Tree::Node const *l = node->left())
        retlist.push(l);
      
      if (typename Tree::Node const *r = node->right())
        retlist.push(r);
    } while (!retlist.empty());
  }
};

template <typename Tree>
class TreeTraverseIterativeHorizontallyWithLevel
: public TreeTraverseBase<Tree>
{
public:
  TreeTraverseIterativeHorizontallyWithLevel(Tree const &tree)
  : TreeTraverseBase<Tree>(tree)
  {}

  template <typename Functor>
  void traverse(Functor const &functor) const {
    typename Tree::Node const *node = TreeTraverseBase<Tree>::tree_.root();

    if (!node)
      return;

    unsigned int level = 0;
    std::queue<typename Tree::Node const *> retlist;
    retlist.push(node);
    retlist.push(nullptr);

    do {
      if (!retlist.front()) {
        ++level;
        retlist.pop();
        if (retlist.empty())
          return;
        else
          retlist.push(nullptr);
      }
        
      typename Tree::Node const *node = retlist.front();
      retlist.pop();

      functor(node, level);

      if (typename Tree::Node const *l = node->left())
        retlist.push(l);
      
      if (typename Tree::Node const *r = node->right())
        retlist.push(r);
    } while (!retlist.empty());
  }
};

template <template <typename> class Strategy, typename Tree, typename Functor>
void traverse_by(Tree const &tree, Functor const &functor) {
  Strategy<Tree>(tree).traverse(functor);
}
