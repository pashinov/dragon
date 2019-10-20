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


class tree_node_stub : public ptree::base_read<traits_stub>
{
public:
    tree_node_stub(traits_stub::node_ptr node) : ptree::base_read<traits_stub>(node), node_(node) { }

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
        reader_.reset(new ptree::base_read<traits_stub>(node_mock_.get()));
    }

    void TearDown() override
    {
    }

protected:
    std::unique_ptr<node_mock> node_mock_;
    std::unique_ptr<ptree::base_read<traits_stub> > reader_;
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