#include "ptree/base_notification.h"
#include "traits_stub.h"

#define UNUSED(value) (void)value

class tree_node_stub : public ptree::base_notification<traits_stub>
{
public:
    tree_node_stub(traits_stub::node_ptr node)
        : ptree::base_notification<traits_stub>(node),
          node_(node) { }

    traits_stub::node_ptr get() const { return node_; }

private:
    traits_stub::node_ptr node_;
};


class base_notification_test : public testing::Test
{
protected:
    void SetUp() override
    {
        node_mock_.reset(new node_mock());
        notify_.reset(new ptree::base_notification<traits_stub>(node_mock_.get()));
    }

    void TearDown() override
    {
    }

protected:
    std::unique_ptr<node_mock> node_mock_;
    std::unique_ptr<ptree::base_notification<traits_stub> > notify_;
};

TEST_F(base_notification_test, child_added)
{
    // Arrange
    notification_object<traits_stub::key_t> child_added_object;

    // Assert
    EXPECT_CALL(*node_mock_.get(), child_added()).WillOnce(::testing::Return(child_added_object));

    // Act
    const notification_object<traits_stub::key_t>& child_added_notify = notify_->child_added();
    UNUSED(child_added_notify);
}

TEST_F(base_notification_test, child_removed)
{
    // Arrange
    notification_object<traits_stub::key_t> child_removed_object;

    // Assert
    EXPECT_CALL(*node_mock_.get(), child_removed()).WillOnce(::testing::Return(child_removed_object));

    // Act
    const notification_object<traits_stub::key_t>& child_removed = notify_->child_removed();
    UNUSED(child_removed);
}

TEST_F(base_notification_test, value_changed)
{
    // Arrange
    notification_object<std::optional<traits_stub::value_t> > value_changed_object;

    // Assert
    EXPECT_CALL(*node_mock_.get(), value_changed()).WillOnce(::testing::Return(value_changed_object));

    // Act
    const notification_object<std::optional<traits_stub::value_t>>& value_changed = notify_->value_changed();
    UNUSED(value_changed);
}
