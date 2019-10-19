// internal
#include "ptree/base_tree.h"
#include "traits_stub.h"

// gtest
#include "gtest/gtest.h"

// gmock
#include "gmock/gmock.h"

// std
#include <variant>
#include <string>
#include <list>


class tree_node_stub : public ptree::base_tree<traits_stub, tree_node_stub>
{
public:
    friend class ptree::base_tree<traits_stub, tree_node_stub>;
    tree_node_stub(traits_stub::node_ptr node)
        : ptree::base_tree<traits_stub, tree_node_stub>(node)
    {
        node_ = node;
    }

    traits_stub::node_ptr get() const { return node_; }

private:
    traits_stub::node_ptr node_;
};


class base_tree_test : public testing::Test
{
protected:
    void SetUp() override
    {
        node_mock_.reset(new node_mock());
        // tree_.reset(new tree_node_stub(node_mock_.get()));
        tree_.reset(new ptree::base_tree<traits_stub, tree_node_stub>(node_mock_.get()));
    }

    void TearDown() override
    {
    }

protected:
    std::unique_ptr<node_mock> node_mock_;
    std::unique_ptr<ptree::base_tree<traits_stub, tree_node_stub>> tree_;
    // std::unique_ptr<tree_node_stub> tree_;
};

TEST_F(base_tree_test, has_childred)
{
    // Arrange in SetUp()

    // Assert
    EXPECT_CALL(*node_mock_.get(), has_children()).WillOnce(::testing::Return(false));

    // Act and Assert
    ASSERT_FALSE(tree_->has_children());

    // node_mock child1 = tree_->child(traits_stub::key_t("child1"));

}

TEST_F(base_tree_test, exist)
{
    // Arrange
    traits_stub::key_t key("child");

    // Assert
    EXPECT_CALL(*node_mock_.get(), exist(key)).WillOnce(::testing::Return(false));

    // Act and Assert
    ASSERT_FALSE(tree_->exist(key));
}

TEST_F(base_tree_test, child_key)
{
    // Arrange
    traits_stub::key_t key("child");

    // Assert
    EXPECT_CALL(*node_mock_.get(), child(key)).WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*node_mock_.get(), add_child(key)).WillOnce(::testing::Return(node_mock_.get()));

    // Act and Assert
    ASSERT_EQ(tree_->child(key).get(), node_mock_.get());

    // Assert
    EXPECT_CALL(*node_mock_.get(), child(key)).WillOnce(::testing::Return(node_mock_.get()));
    EXPECT_CALL(*node_mock_.get(), add_child(::testing::_)).Times(0);

    // Act and Assert
    ASSERT_EQ(tree_->child(key).get(), node_mock_.get());
}

TEST_F(base_tree_test, children)
{
    node_mock node_mock1;
    node_mock node_mock2;
    node_mock node_mock3;

    // Arrange
    std::map<traits_stub::key_t, traits_stub::node_ptr> children1 =
    {
        { "child1", &node_mock1 }, { "child2", &node_mock2}, { "child3", &node_mock3 }
    };

    // Assert
    EXPECT_CALL(*node_mock_.get(), children()).WillOnce(::testing::Return(children1));

    // Act
    auto children2 = tree_->children();

    // Assert
    for(const auto& item1 : children1)
    {
        auto item2 = children2.find(item1.first);
        ASSERT_TRUE(item2 != children2.cend());
        ASSERT_EQ(item1.second, item2->second.get());
    }
}

TEST_F(base_tree_test, child_path)
{
    // Arrange
    using path_t = std::list<traits_stub::key_t>;

    path_t path = {"child1", "child2", "child3"};

    traits_stub::key_t key1 = "child1";
    traits_stub::key_t key2 = "child2";
    traits_stub::key_t key3 = "child3";

    node_mock node_mock1;
    node_mock node_mock2;
    node_mock node_mock3;

    // Assert
    EXPECT_CALL(*node_mock_.get(), child(key1)).WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*node_mock_.get(), add_child(key1)).WillOnce(::testing::Return(&node_mock1));

    EXPECT_CALL(node_mock1, child(key2)).WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(node_mock1, add_child(key2)).WillOnce(::testing::Return(&node_mock2));

    EXPECT_CALL(node_mock2, child(key3)).WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(node_mock2, add_child(key3)).WillOnce(::testing::Return(&node_mock3));

    // Act
    tree_node_stub child = tree_->child<path_t>(path);

    // Assert
    ASSERT_EQ(child.get(), &node_mock3);

    // Assert
    EXPECT_CALL(*node_mock_.get(), child(key1)).WillOnce(::testing::Return(&node_mock1));
    EXPECT_CALL(*node_mock_.get(), add_child(key1)).Times(0);

    EXPECT_CALL(node_mock1, child(key2)).WillOnce(::testing::Return(&node_mock2));
    EXPECT_CALL(node_mock1, add_child(key2)).Times(0);

    EXPECT_CALL(node_mock2, child(key3)).WillOnce(::testing::Return(&node_mock3));
    EXPECT_CALL(node_mock2, add_child(key3)).Times(0);

    // Act
    tree_node_stub child2 = tree_->child<path_t>(path);

    // Assert
    ASSERT_EQ(child2.get(), &node_mock3);
}

TEST_F(base_tree_test, operator_index)
{
    // Arrange
    using path_t = std::list<traits_stub::key_t>;

    path_t path = {"child1", "child2", "child3"};

    traits_stub::key_t key1 = "child1";
    traits_stub::key_t key2 = "child2";
    traits_stub::key_t key3 = "child3";

    node_mock node_mock1;
    node_mock node_mock2;
    node_mock node_mock3;

    // Assert
    EXPECT_CALL(*node_mock_.get(), child(key1)).WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(*node_mock_.get(), add_child(key1)).WillOnce(::testing::Return(&node_mock1));

    EXPECT_CALL(node_mock1, child(key2)).WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(node_mock1, add_child(key2)).WillOnce(::testing::Return(&node_mock2));

    EXPECT_CALL(node_mock2, child(key3)).WillOnce(::testing::Return(nullptr));
    EXPECT_CALL(node_mock2, add_child(key3)).WillOnce(::testing::Return(&node_mock3));

    // Act
    tree_node_stub child = (*tree_.get())[key1][key2][key3];

    // Assert
    ASSERT_EQ(child.get(), &node_mock3);

    // Assert
    EXPECT_CALL(*node_mock_.get(), child(key1)).WillOnce(::testing::Return(&node_mock1));
    EXPECT_CALL(*node_mock_.get(), add_child(key1)).Times(0);

    EXPECT_CALL(node_mock1, child(key2)).WillOnce(::testing::Return(&node_mock2));
    EXPECT_CALL(node_mock1, add_child(key2)).Times(0);

    EXPECT_CALL(node_mock2, child(key3)).WillOnce(::testing::Return(&node_mock3));
    EXPECT_CALL(node_mock2, add_child(key3)).Times(0);

    // Act
    tree_node_stub child2 = (*tree_.get())[key1][key2][key3];

    // Assert
    ASSERT_EQ(child2.get(), &node_mock3);
}
