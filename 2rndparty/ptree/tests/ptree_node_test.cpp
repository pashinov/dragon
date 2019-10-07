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

    auto child1 = root_.child("child1");
    auto child2 = root_.child("child2");
    auto child3 = root_.child("child3");

    ASSERT_TRUE(root_.has_children());

    ASSERT_FALSE(child1.is_root());
    ASSERT_FALSE(child1.has_children());

    ASSERT_FALSE(child2.is_root());
    ASSERT_FALSE(child2.has_children());

    ASSERT_FALSE(child3.is_root());
    ASSERT_FALSE(child3.has_children());

    auto children = root_.children();
    ASSERT_EQ(children.size(), 3);
}

TEST_F(ptree_node_test, create_node_by_path)
{
    auto node = root_.child<std::list<std::string>>({ "a", "b", "c" });
}
