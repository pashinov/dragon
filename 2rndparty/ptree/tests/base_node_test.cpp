// gtest
#include <gtest/gtest.h>

// internal
#include <PropertyTree>
#include "ptree/base_node_error.h"

// std
#include <string>
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
    using base_node_unique_ptr = std::unique_ptr<base_node_t>;
protected:
    void SetUp() override
    {
        root_.reset(base_node_t::root());
    }

    void TearDown() override
    {
        root_.release();
    }

protected:
    base_node_unique_ptr root_ = nullptr;
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
    base_node_traits_t::node_ptr correct_parent = nullptr;
    // Act
    base_node_traits_t::node_ptr obtained_parent = root_->parent();
    // Assert
    ASSERT_EQ(correct_parent, obtained_parent);

    ASSERT_TRUE(root_->empty());
    ASSERT_FALSE(root_->has_children());
    ASSERT_FALSE(root_->has_value());

    const base_node_traits_t::value_t& value1 = root_->value(base_node_traits_t::value_t());
    ASSERT_EQ(value1, base_node_traits_t::value_t());

    try
    {
        const base_node_traits_t::value_t& value2 = root_->value();
        UNUSED(value2);
        FAIL() << "will be exception: base_node_error type";
    }
    catch(const ptree::base_node_error& ex)
    {
        ASSERT_EQ(ex.type(), ptree::base_node_error::error_type::value_not_exist);
    }
    catch(...)
    {
        FAIL() << "other exception, will be base_node_error";
    }

    const base_node_traits_t::children_t& children1 = root_->children(base_node_traits_t::children_t());
    ASSERT_EQ(children1, base_node_traits_t::children_t());
    try
    {
        const base_node_traits_t::children_t& children2 = root_->children();
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
    root_->set(std::uint64_t(5));

    // Assert
    ASSERT_FALSE(root_->empty());
    ASSERT_TRUE(root_->has_value());
    ASSERT_FALSE(root_->has_children());
    ASSERT_EQ(std::get<std::uint64_t>(root_->value()), value);
}