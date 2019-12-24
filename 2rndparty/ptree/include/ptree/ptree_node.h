#ifndef PTREE_NODE_H
#define PTREE_NODE_H

// internal
#include "base_tree.h"
#include "base_read.h"
#include "base_write.h"
#include "base_notification.h"
#include "ptree_const_node.h"

namespace ptree
{
    // class ptree_node is the fasade class (using pattern fasade)
    template <typename Traits>
    class ptree_node : public base_tree<Traits, ptree_node<Traits> >,
                       public base_read<Traits>,
                       public base_write<Traits>,
                       public base_notification<Traits>
    {
        // class base_tree use private contructor to create new object ptree_node
        friend class base_tree<Traits, ptree_node<Traits> >;

        using common_tree_t = base_tree<Traits, ptree_node<Traits> >;
        using reading_tree_t = base_read<Traits>;
        using writing_tree_t = base_write<Traits>;
        using notification_changed_t = base_notification<Traits>;

    public:
        ptree_node();
        ~ptree_node();

        // the common base functionality of the tree
        using common_tree_t::has_children;
        using common_tree_t::exist;
        using common_tree_t::child;
        using common_tree_t::children;
        using common_tree_t::operator[];
        using common_tree_t::operator==;

        // the reading functionality of the tree
        using reading_tree_t::empty;
        using reading_tree_t::has_value;
        using reading_tree_t::operator bool;
        using reading_tree_t::key;
        using reading_tree_t::value;

        // the wrinitg functionality of the tree
        using writing_tree_t::set_value;
        using writing_tree_t::clear;
        using writing_tree_t::erase;
        using writing_tree_t::operator=;

        // the tree change notification
        using notification_changed_t::child_added;
        using notification_changed_t::child_removed;
        using notification_changed_t::value_changed;

        bool operator == (const ptree_node& other) const;
        bool operator != (const ptree_node& other) const;

    private:
        ptree_node(const typename Traits::node_shared_ptr& node);

    protected:
        typename Traits::node_shared_ptr node_;
    };

} // namespace

#endif // PTREE_NODE_H

#include "ptree_node.hpp"
