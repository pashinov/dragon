// gtest
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// internal
#include <NotificationObject>

#include "notification_object/storage_action.h"
#include "notification_object/notification_token.h"
#include "notification_object/notification_handle.h"

// std
#include <string>
#include <memory>

class storage_action_mock : public storage_action
{
public:
    MOCK_METHOD1(remove, void(notification_token_ptr));
};

class notification_handle_test : public ::testing::Test
{
public:
    ~notification_handle_test () override = default;

protected:
    void SetUp() override
    {
        storage_action_mock_.reset(new storage_action_mock());
        token_.reset(new notification_token(storage_action_mock_.get()));
        handle_.reset(new notification_handle(token_.get()));
    }

    void TearDown() override { }

    std::unique_ptr<storage_action_mock> storage_action_mock_;
    std::unique_ptr<notification_token> token_;
    std::unique_ptr<notification_handle> handle_;
};

TEST_F(notification_handle_test, disconnect_test)
{
    // Arrange in SetUp()

    // Assert
    EXPECT_CALL(*storage_action_mock_.get(), remove(token_.get()));

    // Act
    handle_->disconnect();
}
