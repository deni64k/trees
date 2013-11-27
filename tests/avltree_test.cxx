#include <avltree.hxx>

#include "test.hxx"

class AVLTreeTest
: public Test {
protected:
  AVLTreeTest()
  : tree_()
  {}

  AVLTree<int> tree_;
};

TEST_F(AVLTreeTest, perform) {
  for (int i = 1; i <= 512; ++i) {
    tree_.insert(i);
  }
  // {
  //   std::vector<int> data{};
  //   sorter_.perform(data.begin(), data.end());
  //   EXPECT_EQ((std::vector<int>{}), data);
  //   EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
  // }

  // {
  //   std::vector<int> data{1};
  //   sorter_.perform(data.begin(), data.end());
  //   EXPECT_EQ((std::vector<int>{1}), data);
  //   EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
  // }

  // {
  //   std::vector<int> data{5, 4, 3, 2, 1};
  //   sorter_.perform(data.begin(), data.end());
  //   EXPECT_EQ((std::vector<int>{1, 2, 3, 4, 5}), data);
  //   EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
  // }
}
