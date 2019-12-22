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


class base_tree_node_stub : public ptree::base_tree<traits_stub, base_tree_node_stub>
{
    using common_tree_t = ptree::base_tree<traits_stub, base_tree_node_stub>;

    friend class ptree::base_tree<traits_stub, base_tree_node_stub>;
public:
    base_tree_node_stub(const traits_stub::node_shared_ptr& node)
        : ptree::base_tree<traits_stub, base_tree_node_stub>(node),
          node_(node)

    {
    }

//    bool operator == (const base_tree_node_stub& other) const
//    {
//        return common_tree_t::operator==(other);
//    }
//    bool operator != (const base_tree_node_stub& other) const
//    {
//        return common_tree_t::operator != (other);
//    }

    traits_stub::node_weak_ptr get() const { return node_; }

private:
    traits_stub::node_shared_ptr node_;
};


class base_tree_test : public testing::Test
{
protected:
    base_tree_test() : node_mock_(new node_mock()), tree_(new base_tree_node_stub(node_mock_)) { }

    void SetUp() override { }

    void TearDown() override { }

protected:
    std::shared_ptr<node_mock> node_mock_;
    std::shared_ptr<base_tree_node_stub> tree_;
};

TEST_F(base_tree_test, has_childred)
{
    // Arrange in SetUp()

    // Assert
    EXPECT_CALL(*node_mock_.get(), has_children()).WillOnce(::testing::Return(false));

    // Act and Assert
    ASSERT_FALSE(tree_->has_children());
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
    EXPECT_CALL(*node_mock_.get(), child(key)).WillOnce(::testing::Return(traits_stub::node_weak_ptr()));
    EXPECT_CALL(*node_mock_.get(), add_child(key)).WillOnce(::testing::Return(std::weak_ptr<node_mock>(node_mock_)));

    // Act and Assert
    base_tree_node_stub child1 = tree_->child(key);
    ASSERT_EQ(child1.get().lock(), node_mock_);

    // Assert
    std::weak_ptr<node_mock> child_res = node_mock_;
    EXPECT_CALL(*node_mock_.get(), child(key)).WillOnce(::testing::Return(child_res));
    EXPECT_CALL(*node_mock_.get(), add_child(::testing::_)).Times(0);

    // Act and Assert
    base_tree_node_stub child2 = tree_->child(key);
    ASSERT_EQ(child2.get().lock(), node_mock_);
}

TEST_F(base_tree_test, children)
{
    std::shared_ptr<node_mock> node_mock1(new node_mock());
    std::shared_ptr<node_mock> node_mock2(new node_mock());
    std::shared_ptr<node_mock> node_mock3(new node_mock());

    // Arrange
    std::map<traits_stub::key_t, traits_stub::node_shared_ptr> children1 =
    {
        { "child1", node_mock1 }, { "child2", node_mock2}, { "child3", node_mock3 }
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
        ASSERT_EQ(item1.second, item2->second.get().lock());
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

    std::shared_ptr<node_mock> node_mock1(new node_mock());
    std::shared_ptr<node_mock> node_mock2(new node_mock());
    std::shared_ptr<node_mock> node_mock3(new node_mock());

    // Assert
    EXPECT_CALL(*node_mock_, child(key1)).WillOnce(::testing::Return(traits_stub::node_weak_ptr()));
    EXPECT_CALL(*node_mock_, add_child(key1)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock1)));

    EXPECT_CALL(*node_mock1, child(key2)).WillOnce(::testing::Return(traits_stub::node_weak_ptr()));
    EXPECT_CALL(*node_mock1, add_child(key2)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock2)));

    EXPECT_CALL(*node_mock2, child(key3)).WillOnce(::testing::Return(traits_stub::node_weak_ptr()));
    EXPECT_CALL(*node_mock2, add_child(key3)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock3)));

    // Act
    base_tree_node_stub child = tree_->child<path_t>(path);

    // Assert
    ASSERT_EQ(child.get().lock(), node_mock3);

    // Assert
    EXPECT_CALL(*node_mock_, child(key1)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock1)));
    EXPECT_CALL(*node_mock_, add_child(key1)).Times(0);

    EXPECT_CALL(*node_mock1, child(key2)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock2)));
    EXPECT_CALL(*node_mock1, add_child(key2)).Times(0);

    EXPECT_CALL(*node_mock2, child(key3)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock3)));
    EXPECT_CALL(*node_mock2, add_child(key3)).Times(0);

    // Act
    base_tree_node_stub child2 = tree_->child<path_t>(path);

    // Assert
    ASSERT_EQ(child2.get().lock(), node_mock3);
}

TEST_F(base_tree_test, operator_index)
{
    // Arrange
    using path_t = std::list<traits_stub::key_t>;

    path_t path = {"child1", "child2", "child3"};

    traits_stub::key_t key1 = "child1";
    traits_stub::key_t key2 = "child2";
    traits_stub::key_t key3 = "child3";

    std::shared_ptr<node_mock> node_mock1(new node_mock());
    std::shared_ptr<node_mock> node_mock2(new node_mock());
    std::shared_ptr<node_mock> node_mock3(new node_mock());

    // Assert
    EXPECT_CALL(*node_mock_, child(key1)).WillOnce(::testing::Return(traits_stub::node_weak_ptr()));
    EXPECT_CALL(*node_mock_, add_child(key1)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock1)));

    EXPECT_CALL(*node_mock1, child(key2)).WillOnce(::testing::Return(traits_stub::node_weak_ptr()));
    EXPECT_CALL(*node_mock1, add_child(key2)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock2)));

    EXPECT_CALL(*node_mock2, child(key3)).WillOnce(::testing::Return(traits_stub::node_weak_ptr()));
    EXPECT_CALL(*node_mock2, add_child(key3)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock3)));

    // Act
    base_tree_node_stub child = (*tree_)[key1][key2][key3];

    // Assert
    ASSERT_EQ(child.get().lock(), node_mock3);

    // Assert
    EXPECT_CALL(*node_mock_, child(key1)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock1)));
    EXPECT_CALL(*node_mock_, add_child(key1)).Times(0);

    EXPECT_CALL(*node_mock1, child(key2)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock2)));
    EXPECT_CALL(*node_mock1, add_child(key2)).Times(0);

    EXPECT_CALL(*node_mock2, child(key3)).WillOnce(::testing::Return(traits_stub::node_weak_ptr(node_mock3)));
    EXPECT_CALL(*node_mock2, add_child(key3)).Times(0);

    // Act
    base_tree_node_stub child2 = (*tree_)[key1][key2][key3];

    // Assert
    ASSERT_EQ(child2.get().lock(), node_mock3);
}
