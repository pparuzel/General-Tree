#include "gtest/gtest.h"
#include "../../GeneralTree/GeneralTree.hpp"


// General Tests
TEST(BasicTests, EmptyTreeTests)
{
    rcn::GeneralTree<int> empty_tree{};

    ASSERT_EQ(empty_tree.root(), rcn::GeneralTree<int>::iterator{});
    ASSERT_EQ(empty_tree.getNumberOfChildren(empty_tree.root()), 0);
    ASSERT_EQ(empty_tree.size(), 0);
    ASSERT_EQ(empty_tree.root(), empty_tree.end());
    ASSERT_NO_THROW(empty_tree.clear());
}

TEST(BasicTests, OneElemTreeTests)
{
    rcn::GeneralTree<int> tree{7};

    EXPECT_NO_THROW(tree.clear());
    EXPECT_EQ(tree.root(), tree.end());
    EXPECT_NO_THROW(tree.insert(8, tree.end(), 0));
    EXPECT_EQ(8, *tree.root());
}
