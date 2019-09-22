// gtest
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// internal
#include <NotificationObject>

// std
#include <string>

class callbacks_mock
{
public:
    MOCK_METHOD0(on_param_0_function, void());
    MOCK_METHOD1(on_param_1_function, void(int));
    MOCK_METHOD2(on_param_2_function, void(const std::string&, int));
    MOCK_METHOD3(on_param_3_function, void(const std::string&, int, double));
};

class notification_object_test : public ::testing::Test
{
public:
    ~notification_object_test () override { }

protected:
    void SetUp() override { }

    void TearDown() override { }

    callbacks_mock callbacks_;

    int int_value_            { 1 };
    std::string string_value_ { "test" };
    double double_value_      { 5.8 };

    notification_object<> object_;
    notification_object<int> object1_;
    notification_object<std::string, int> object2_;
    notification_object<std::string, int, double> object3_;
};

TEST_F(notification_object_test, connect_test)
{
    // Act
    object_.connect(std::bind(&callbacks_mock::on_param_0_function, &callbacks_));
    object1_.connect(std::bind(&callbacks_mock::on_param_1_function, &callbacks_, std::placeholders::_1));
    object2_.connect(std::bind(&callbacks_mock::on_param_2_function, &callbacks_, std::placeholders::_1,
                               std::placeholders::_2));
    object3_.connect(std::bind(&callbacks_mock::on_param_3_function, &callbacks_, std::placeholders::_1,
                               std::placeholders::_2, std::placeholders::_3));

    // Assert
    EXPECT_CALL(callbacks_, on_param_0_function()).Times(1);
    EXPECT_CALL(callbacks_, on_param_1_function(int_value_)).Times(1);
    EXPECT_CALL(callbacks_, on_param_2_function(string_value_, int_value_)).Times(1);
    EXPECT_CALL(callbacks_, on_param_3_function(string_value_, int_value_, double_value_)).Times(1);

    // Act
    object_.notify();
    object1_.notify(int_value_);
    object2_.notify(string_value_, int_value_);
    object3_.notify(string_value_, int_value_, double_value_);
}

TEST_F(notification_object_test, connect_test2)
{
    // Act
    object_.connect(&callbacks_, &callbacks_mock::on_param_0_function);
    object1_.connect(&callbacks_, &callbacks_mock::on_param_1_function);
    object2_.connect(&callbacks_, &callbacks_mock::on_param_2_function);
    object3_.connect(&callbacks_, &callbacks_mock::on_param_3_function);

    // Assert
    EXPECT_CALL(callbacks_, on_param_0_function()).Times(1);
    EXPECT_CALL(callbacks_, on_param_1_function(int_value_)).Times(1);
    EXPECT_CALL(callbacks_, on_param_2_function(string_value_, int_value_)).Times(1);
    EXPECT_CALL(callbacks_, on_param_3_function(string_value_, int_value_, double_value_)).Times(1);

    // Act
    object_.notify();
    object1_.notify(int_value_);
    object2_.notify(string_value_, int_value_);
    object3_.notify(string_value_, int_value_, double_value_);
}

TEST_F(notification_object_test, disconnect_test)
{
    // Arrange
    notification_handle handle = object_.connect(&callbacks_, &callbacks_mock::on_param_0_function);
    notification_handle handle1 = object1_.connect(&callbacks_, &callbacks_mock::on_param_1_function);
    notification_handle handle2 = object2_.connect(&callbacks_, &callbacks_mock::on_param_2_function);
    notification_handle handle3 = object3_.connect(&callbacks_, &callbacks_mock::on_param_3_function);

    // Act
    handle.disconnect();
    handle1.disconnect();
    handle2.disconnect();
    handle3.disconnect();

    // Assert
    EXPECT_CALL(callbacks_, on_param_0_function()).Times(0);
    EXPECT_CALL(callbacks_, on_param_1_function(int_value_)).Times(0);
    EXPECT_CALL(callbacks_, on_param_2_function(string_value_, int_value_)).Times(0);
    EXPECT_CALL(callbacks_, on_param_3_function(string_value_, int_value_, double_value_)).Times(0);

    object_.notify();
    object1_.notify(int_value_);
    object2_.notify(string_value_, int_value_);
    object3_.notify(string_value_, int_value_, double_value_);
}
