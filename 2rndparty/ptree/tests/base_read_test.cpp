// internal
#include "ptree/base_read.h"
#include "traits_stub.h"

// gtest
#include "gtest/gtest.h"

// gmock
#include "gmock/gmock.h"

// std
#include <variant>
#include <string>


class tree_node_read_stub : private ptree::base_read<traits_stub>
{
public:
    tree_node_read_stub(traits_stub::node_ptr node)
        : ptree::base_read<traits_stub>(node), node_(node) { }

    using reading_tree_t = base_read<traits_stub>;

    using reading_tree_t::empty;
    using reading_tree_t::has_value;
    using reading_tree_t::operator bool;
    using reading_tree_t::key;
    using reading_tree_t::value;

    traits_stub::node_ptr get() const { return node_; }

private:
    traits_stub::node_ptr node_;
};


class base_read_test : public testing::Test
{
protected:
    void SetUp() override
    {
        node_mock_.reset(new node_mock());
        reader_.reset(new tree_node_read_stub(node_mock_.get()));
    }

    void TearDown() override
    {
    }

protected:
    std::unique_ptr<node_mock> node_mock_;
    std::unique_ptr<tree_node_read_stub> reader_;
};

TEST_F(base_read_test, empty)
{
    // Arrange in SetUp()
    // Assert
    EXPECT_CALL(*node_mock_.get(), empty()).WillOnce(::testing::Return(true));

    // Act and Assert
    ASSERT_TRUE(reader_->empty());

    // Assert
    EXPECT_CALL(*node_mock_.get(), empty()).WillOnce(::testing::Return(false));

    // Act and Assert
    ASSERT_FALSE(reader_->empty());

}

TEST_F(base_read_test, has_value)
{
    // Arrange in SetUp()
    // Assert
    EXPECT_CALL(*node_mock_.get(), has_value()).WillOnce(::testing::Return(true));

    // Act and Assert
    ASSERT_TRUE(reader_->has_value());

    // Assert
    EXPECT_CALL(*node_mock_.get(), has_value()).WillOnce(::testing::Return(false));

    // Act and Assert
    ASSERT_FALSE(reader_->has_value());
}

TEST_F(base_read_test, operator_to_bool_convertion)
{
    // Arrange in SetUp()
    // Assert
    EXPECT_CALL(*node_mock_.get(), empty()).WillOnce(::testing::Return(true));

    // Act and Assert
    ASSERT_FALSE(bool(*reader_.get()));

    // Assert
    EXPECT_CALL(*node_mock_.get(), empty()).WillOnce(::testing::Return(false));

    // Act and Assert
    ASSERT_TRUE(bool(*reader_.get()));
}

TEST_F(base_read_test, key)
{
    // Arrange
    traits_stub::key_t key = "child";

    // Assert
    EXPECT_CALL(*node_mock_.get(), key()).WillOnce(::testing::Return(key));

    // Act and Assert
    ASSERT_EQ(key, reader_->key());
}

TEST_F(base_read_test, value)
{
    // TODO: !mock object has not set_value(), when value_t is std::variant

    // Arrange
    traits_stub::value_t value = std::uint64_t(5);

    // EXPECT_CALL(*node_mock_.get(), value()).Times(1);

    // Act and Assert
    // reader_->value();
    // ASSERT_EQ(std::get<std::uint64_t>(value), std::get<uint64_t>(reader_->value()));
}
