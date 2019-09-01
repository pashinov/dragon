// gtest
#include <gtest/gtest.h>

// internal
#include <PropertyTree>

// std
#include <string>

class base_node_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

protected:
    ptree::base_node<ptree::traits<std::string, std::string> > root_ { "root" };
};

TEST_F(base_node_test, create_node)
{
}