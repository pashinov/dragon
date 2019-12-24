// gtest
#include <gtest/gtest.h>

// gmock
#include <gmock/gmock.h>

// internal
#include <PropertyTree>
#include <NotificationObject>
#include "ptree/base_node_error.h"

// std
#include <string>
#include <map>
#include <variant>
#include <memory>

#define UNUSED(value) (void)value

class base_node_test : public ::testing::Test
{
public:
    using base_node_value_t = std::variant<std::monostate, std::int64_t, std::uint64_t, std::string, double, float>;
    using base_node_traits_t = ptree::traits<std::string, base_node_value_t>;
    using base_node_t = ptree::base_node<base_node_traits_t>;
    using base_node_ptr = base_node_t*;
    using base_node_shared_ptr = std::shared_ptr<base_node_t>;
    using base_node_weak_ptr = std::weak_ptr<base_node_t>;
protected:
    void SetUp() override
    {
        root_ = base_node_t::root();
    }

    void TearDown() override
    {
        root_.reset();
    }

protected:
    base_node_shared_ptr root_ = nullptr;
};

TEST_F(base_node_test, root_node)
{
    // Arrange
    base_node_traits_t::key_t correct_key = base_node_traits_t::key_t();
    // Act
    base_node_traits_t::key_t obtained_key = root_->key();
    // Assert
    ASSERT_EQ(correct_key, obtained_key);

    // Arrange
    base_node_traits_t::node_weak_ptr correct_parent;
    // Act
    base_node_traits_t::node_weak_ptr obtained_parent = root_->parent();
    // Assert
    ASSERT_EQ(correct_parent.lock(), obtained_parent.lock());

    ASSERT_TRUE(root_->empty());
    ASSERT_FALSE(root_->has_children());
    ASSERT_FALSE(root_->has_value());

    const base_node_traits_t::optional_value_t& value = root_->value();
    ASSERT_FALSE(value.has_value());

    std::map<base_node_traits_t::key_t, base_node_traits_t::node_shared_ptr> default_value = { };
    const std::map<base_node_traits_t::key_t, base_node_traits_t::node_shared_ptr>& children1
            = root_->children(default_value);
    ASSERT_EQ(children1, default_value);
    try
    {
        const std::map<base_node_traits_t::key_t, base_node_traits_t::node_shared_ptr>& children2
                = root_->children();
        UNUSED(children2);
        FAIL() << "will be exception: base_node_error type";
    }
    catch(const ptree::base_node_error& ex)
    {
        ASSERT_EQ(ex.type(), ptree::base_node_error::error_type::children_not_exist);
    }
    catch(...)
    {
        FAIL() << "other exception, will be base_node_error";
    }
}

TEST_F(base_node_test, set_value)
{
    // Arrange
    std::uint64_t value = 5;

    // Act
    root_->set_value(std::uint64_t(5));

    // Assert
    ASSERT_FALSE(root_->empty());
    ASSERT_TRUE(root_->has_value());
    ASSERT_FALSE(root_->has_children());
    const auto& v = root_->value();
    ASSERT_TRUE(v.has_value());
    ASSERT_EQ(std::get<std::uint64_t>(v.value()), value);
}

TEST_F(base_node_test, add_children)
{
    // Arrange
    base_node_traits_t::key_t key1 = { "child1" };
    base_node_traits_t::key_t key2 = { "child2" };

    // Act
    base_node_traits_t::node_shared_ptr child1 = root_->add_child(key1);
    base_node_traits_t::node_shared_ptr child2 = root_->add_child(key2, { std::uint64_t(10) });

    // Assert
    ASSERT_TRUE(child1.get());
    ASSERT_TRUE(child2.get());

    ASSERT_EQ(root_.get(), child1->parent().lock().get());
    ASSERT_EQ(root_.get(), child2->parent().lock().get());

    ASSERT_EQ(child1->key(), key1);
    ASSERT_EQ(child2->key(), key2);

    ASSERT_TRUE(child1->empty());
    ASSERT_FALSE(child2->empty());

    ASSERT_FALSE(child1->has_value());
    ASSERT_TRUE(child2->has_value());

    ASSERT_FALSE(child1->has_children());
    ASSERT_FALSE(child2->has_children());

    base_node_traits_t::node_shared_ptr child1_shared_ptr = root_->child(key1);
    base_node_traits_t::node_shared_ptr child2_shared_ptr = root_->child(key2);

    ASSERT_TRUE(child1_shared_ptr.get());
    ASSERT_TRUE(child2_shared_ptr.get());
    ASSERT_EQ(child1_shared_ptr.get(), child1.get());
    ASSERT_EQ(child2_shared_ptr.get(), child2.get());
    ASSERT_EQ(root_->child(base_node_traits_t::key_t("child3")).get(), nullptr);
}

TEST_F(base_node_test, clear_children)
{
    // Arrange
    base_node_traits_t::key_t key1 = { "child1" };
    base_node_traits_t::key_t key2 = { "child2" };

    root_->add_child(key1);
    root_->add_child(key2);

    // Act
    root_->clear();

    // Assert
    ASSERT_TRUE(root_->empty());
    ASSERT_FALSE(root_->has_children());
    ASSERT_FALSE(root_->has_value());
    ASSERT_EQ(root_->child(key1), nullptr);
    ASSERT_EQ(root_->child(key2), nullptr);
    ASSERT_FALSE(root_->exist(key1));
    ASSERT_FALSE(root_->exist(key2));
}

TEST_F(base_node_test, clear_value)
{
    // Arrange
    bool was_set_value = root_->set_value(std::int64_t(5));

    // Act
    root_->clear();

    // Assert
    ASSERT_TRUE(was_set_value);
    ASSERT_TRUE(root_->empty());
    ASSERT_FALSE(root_->has_value());
    ASSERT_FALSE(root_->has_children());
    auto value = root_->value();
    ASSERT_FALSE(value.has_value());
}

class notification_mock
{
public:
    MOCK_METHOD1(on_value_changed_mock, void(const std::optional<base_node_test::base_node_traits_t::value_t>&));
    MOCK_METHOD1(on_child_added_mock, void(const base_node_test::base_node_traits_t::key_t&));
    MOCK_METHOD1(on_child_removed_mock, void(const base_node_test::base_node_traits_t::key_t&));

    void on_value_changed(const std::optional<base_node_test::base_node_traits_t::value_t>& new_value)
    {
        this->on_value_changed_mock(new_value);
    }

    void on_child_added(const base_node_test::base_node_traits_t::key_t& key)
    {
        this->on_child_added_mock(key);
    }

    void on_child_removed(const base_node_test::base_node_traits_t::key_t& key)
    {
        this->on_child_removed_mock(key);
    }
};

TEST_F(base_node_test, value_changed)
{
    // Arrange
    notification_mock mock;
    std::uint64_t value = 5;
    root_->value_changed().connect(&mock, &notification_mock::on_value_changed);

    // Assert
    std::optional<base_node_test::base_node_value_t> opt_value = value;
    EXPECT_CALL(mock, on_value_changed_mock(opt_value)).Times(1);

    // Act
    root_->set_value(value);
}

TEST_F(base_node_test, child_added)
{
    // Arrange
    notification_mock mock;
    base_node_traits_t::key_t child = "child1";
    auto handle = root_->child_added().connect(&mock, &notification_mock::on_child_added);

    // Assert
    EXPECT_CALL(mock, on_child_added_mock(child)).Times(1);

    // Act
    root_->add_child(child);

    // Act
    handle.disconnect();

    // Assert
    EXPECT_CALL(mock, on_child_added_mock(child)).Times(0);
}

TEST_F(base_node_test, child_removed)
{
    // Arrange
    notification_mock mock;
    base_node_traits_t::key_t child = "child1";
    root_->add_child(child);
    auto handle = root_->child_removed().connect(&mock, &notification_mock::on_child_removed);

    // Assert
    EXPECT_CALL(mock, on_child_removed_mock(child)).Times(1);

    // Act
    root_->erase(child);

    // Act
    handle.disconnect();

    // Assert
    EXPECT_CALL(mock, on_child_removed_mock(child)).Times(0);
}
