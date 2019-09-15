// gtest
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// internal
#include <NotificationObject>


class notification_object_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

class callbacks_mock
{
public:
    MOCK_METHOD0(on_object_changed, void());
};

TEST_F(notification_object_test, connect_test)
{
    // Arrange
    notification_object<> object;
    callbacks_mock callback;

    // Act
    object.connect(std::bind(&callbacks_mock::on_object_changed, &callback));

    EXPECT_CALL(callback, on_object_changed()).Times(1);

    object.notify();
}