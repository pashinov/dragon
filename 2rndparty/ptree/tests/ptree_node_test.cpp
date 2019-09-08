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
}