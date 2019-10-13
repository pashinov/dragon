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

private:
    std::unique_ptr<node_mock> node_mock_;
    std::unique_ptr<ptree::base_read<traits_stub> > reader_;
};

TEST_F(base_read_test, base_test)
{

}
