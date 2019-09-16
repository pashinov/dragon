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
    MOCK_METHOD3(on_param_3_function, void(const std::string&, const int&, const double&));
};

class notification_object_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

    callbacks_mock callbacks_;
    int int_value_                   { 1 };
    std::string string_value_ { "test" };
    double double_value_           { 5.8 };
};

TEST_F(notification_object_test, connect_test)
{
    // Arrange
    notification_object<> object;
    notification_object<int> object1;
    notification_object<std::string, int> object2;
    notification_object<std::string, int, double> object3;

    // Act
    object.connect(std::bind(&callbacks_mock::on_param_0_function, &callbacks_));
    object1.connect(std::bind(&callbacks_mock::on_param_1_function, &callbacks_, std::placeholders::_1));
    object2.connect(std::bind(&callbacks_mock::on_param_2_function, &callbacks_, std::placeholders::_1,
            std::placeholders::_2));
    object3.connect(std::bind(&callbacks_mock::on_param_3_function, &callbacks_, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3));

    // Assert
    EXPECT_CALL(callbacks_, on_param_0_function()).Times(1);
    EXPECT_CALL(callbacks_, on_param_1_function(int_value_)).Times(1);
    EXPECT_CALL(callbacks_, on_param_2_function(string_value_, int_value_)).Times(1);
    EXPECT_CALL(callbacks_, on_param_3_function(string_value_, int_value_, double_value_)).Times(1);

    // Act
    object.notify();
    object1.notify(int_value_);
    object2.notify(string_value_, int_value_);
    object3.notify(string_value_, int_value_, double_value_);
}

TEST_F(notification_object_test, connect_test2)
{
    // Arrange
    notification_object<> object;
    notification_object<int> object1;
    notification_object<std::string, int> object2;
    notification_object<std::string, int, double> object3;

    // Act
    object.connect(&callbacks_, &callbacks_mock::on_param_0_function);
    object1.connect(&callbacks_, &callbacks_mock::on_param_1_function);
    object2.connect(&callbacks_, &callbacks_mock::on_param_2_function);
    object3.connect(&callbacks_, &callbacks_mock::on_param_3_function);

    // Assert
    EXPECT_CALL(callbacks_, on_param_0_function()).Times(1);
    EXPECT_CALL(callbacks_, on_param_1_function(int_value_)).Times(1);
    EXPECT_CALL(callbacks_, on_param_2_function(string_value_, int_value_)).Times(1);
    EXPECT_CALL(callbacks_, on_param_3_function(string_value_, int_value_, double_value_)).Times(1);

    // Act
    object.notify();
    object1.notify(int_value_);
    object2.notify(string_value_, int_value_);
    object3.notify(string_value_, int_value_, double_value_);
}