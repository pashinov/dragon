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

class node_stub
{
public:

};

class base_tree_test : public testing::Test
{
protected:
    void SetUp() override
    {
        node_mock_.reset(new node_mock());
        tree_.reset(new ptree::base_tree<traits_stub, node_stub>(node_mock_.get()));
    }

    void TearDown() override
    {
    }

private:
    std::unique_ptr<node_mock> node_mock_;
    std::unique_ptr<ptree::base_tree<traits_stub, node_stub>> tree_;
};

TEST_F(base_tree_test, base_test)
{

}

