//// internal
//#include "ptree/base_write.h"
//#include "traits_stub.h"

//// gtest
//#include "gtest/gtest.h"

//// gmock
//#include "gmock/gmock.h"

//// std
//#include <variant>
//#include <string>
//#include <list>


//class write_tree_node_stub : public ptree::base_write<traits_stub>
//{
//public:
//    write_tree_node_stub(traits_stub::node_weak_ptr node)
//        : ptree::base_write<traits_stub>(node),
//          node_(node) { }

//    traits_stub::node_weak_ptr get() const { return node_; }

//private:
//    traits_stub::node_weak_ptr node_;
//};


//class base_write_test : public testing::Test
//{
//protected:
//    void SetUp() override
//    {
//        node_mock_.reset(new node_mock());
////        writer_.reset(new write_tree_node_stub(node_mock_.get()));
//    }

//    void TearDown() override
//    {
//    }

//protected:
//    std::unique_ptr<node_mock> node_mock_;
//    std::unique_ptr<write_tree_node_stub> writer_;
//};


//TEST_F(base_write_test, clear)
//{
//    // Arrange in SetUp()
//    // Assert
//    EXPECT_CALL(*node_mock_.get(), clear()).Times(1);

//    // Act
//    writer_->clear();
//}

//TEST_F(base_write_test, erise)
//{
//    // Arrange
//    traits_stub::key_t key("child");
//    // Assert
//    EXPECT_CALL(*node_mock_.get(), erase(key));

//    writer_->erase(key);
//}

//TEST_F(base_write_test, set_value)
//{
//    // TODO: !mock object has not set_value(), when value_t is std::variant
//}

//TEST_F(base_write_test, operator_assignment)
//{
//    // TODO: !mock object has not set_value(), when value_t is std::variant
//    // writer_.get()->operator=<traits_stub::value_t>(std::uint64_t(5));
//}
