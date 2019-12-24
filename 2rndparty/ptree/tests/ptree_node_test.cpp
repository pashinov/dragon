// gtest
#include <gtest/gtest.h>
// gmock
#include <gmock/gmock.h>

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
    ASSERT_FALSE(root_.has_children());

    // Act
    auto child1 = root_.child("child1");
    auto child2 = root_.child("child2");
    auto child3 = root_.child("child3");

    // Assert
    ASSERT_TRUE(root_.has_children());
    ASSERT_FALSE(child1.has_children());
    ASSERT_FALSE(child2.has_children());
    ASSERT_FALSE(child3.has_children());

    // Act
    auto children = root_.children();

    // Assert
    ASSERT_EQ(children.size(), 3);
}

TEST_F(ptree_node_test, create_node_by_path)
{
    auto node = root_.child<path_t>({ "a", "b", "c" });
    auto node2 = root_["a"]["b"]["c"];

    ASSERT_TRUE(node == node2);
    ASSERT_FALSE(node != node2);
}

class ptree_notification_mock
{
public:
    MOCK_METHOD1(on_child_added, void(const std::string&));
    MOCK_METHOD1(on_value_changed, void(const std::optional<std::string>&));
};

TEST_F(ptree_node_test, write_node)
{
    // Arange
    ptree_notification_mock notify_mock;
    std::string key1 = "a";

    root_.child_added().connect([&notify_mock](const std::string& key)
    {
        std::cout << key << std::endl;
        notify_mock.on_child_added(key);
    });

    // Asset
    EXPECT_CALL(notify_mock, on_child_added(key1));

    // Act
    auto a = root_.child(key1);

    // Arange
    std::string key2 = "a1";
    a.child_added().connect([&notify_mock](const std::string& key)
    {
        std::cout << key << std::endl;
        notify_mock.on_child_added(key);
    });

    // Assert
    EXPECT_CALL(notify_mock, on_child_added(key2));

    // Act
    auto a1 = root_[key2];

    // Arrange
    std::optional<std::string> value = "value";
    a.value_changed().connect([&notify_mock](const std::optional<std::string>& v)
    {
        notify_mock.on_value_changed(v);
    });

    // Assert
    EXPECT_CALL(notify_mock, on_value_changed(value));

    // Act
    ASSERT_TRUE(a.set_value(value.value()));
}

TEST_F(ptree_node_test, to_ptree_const_node)
{
    // Arrange
    ptree::ptree_node<ptree::traits<std::string, std::string> > ptree_node;

    std::string key1 = "a";
    std::string key2 = "b";
    std::string value1 = "value1";
    std::string value2= "value2";

    auto a = ptree_node[key1];
    auto b = ptree_node[key2];

    ASSERT_TRUE(a.set_value(value1));
    ASSERT_TRUE(b.set_value(value2));

    // Act
    ptree::ptree_const_node<ptree::traits<std::string, std::string> > node_const = ptree_node;
    auto a1 = node_const[key1];
    auto b1 = node_const[key2];

    // Assert
    ASSERT_EQ(a.key(), a1.key());
    ASSERT_EQ(a.value(), a1.value());

    ASSERT_EQ(b.key(), b1.key());
    ASSERT_EQ(b.value(), b1.value());

}
