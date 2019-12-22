// gtest
#include <gtest/gtest.h>

// internal
#include <PropertyTree>

// std
#include <iostream>
#include <string>

using path_t = std::list<std::string>;

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

void on_value_changed(const std::string& value)
{
    std::cout << "on value changed: " << value << std::endl;
}

TEST_F(ptree_node_test, create_node)
{
    // Assert
//    ASSERT_TRUE(root_.is_root());
    ASSERT_FALSE(root_.has_children());

    auto child1 = root_.child("child1");
    auto child2 = root_.child("child2");
    auto child3 = root_.child("child3");

    ASSERT_TRUE(root_.has_children());

//    ASSERT_FALSE(child1.is_root());
    ASSERT_FALSE(child1.has_children());

//    ASSERT_FALSE(child2.is_root());
    ASSERT_FALSE(child2.has_children());

//    ASSERT_FALSE(child3.is_root());
    ASSERT_FALSE(child3.has_children());

    auto children = root_.children();
    ASSERT_EQ(children.size(), 3);
}

//TEST_F(ptree_node_test, create_node_by_path)
//{
//    auto node = root_.child<path_t>({ "a", "b", "c" });
//    auto node2 = root_["a"]["b"]["c"];

//    ASSERT_TRUE(node == node2);
//    ASSERT_FALSE(node != node2);
//}

//TEST_F(ptree_node_test, write_node)
//{
//    root_.child_added().connect([](const std::string& key)
//    {
//        std::cout << key << std::endl;
//    });
//    ptree::ptree_node<ptree::traits<std::string, std::string> > a = root_["a"];
//    a.child_added().connect([](const std::string& key)
//    {
//        std::cout << key << std::endl;
//    });
//    auto b = a["b"];
//    auto c = a["c"];
//    auto d = a["d"];

//    b.value_changed().connect([](const std::optional<std::string>& v)
//    {
//        std::cout << v.value() << std::endl;
//    });

//    b.set_value("val2");
//    b.set_value("dsf");
//}
