// gtest
#include <gtest/gtest.h>

// internal
#include <PropertyTree>

// std
#include <string>

class ptree_node_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

protected:
    ptree::ptree_node<ptree::traits<std::string, std::string> > root_;
};

TEST_F(ptree_node_test, create_node)
{
    // Assert
    ASSERT_TRUE(root_.is_root());
    ASSERT_FALSE(root_.has_children());

    auto child = root_.child("child1");

    ASSERT_FALSE(child.is_root());
    ASSERT_TRUE(root_.has_children());
    ASSERT_FALSE(child.has_children());
}
