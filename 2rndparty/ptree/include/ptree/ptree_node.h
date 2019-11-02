#ifndef PTREE_NODE_H
#define PTREE_NODE_H

// internal
#include "base_tree.h"
#include "base_read.h"
#include "base_write.h"
#include "base_notification.h"

// std
#include <memory>

namespace ptree
{
    template <typename Traits>
    class ptree_node : public base_tree<Traits, ptree_node<Traits> >,
                       public base_read<Traits>,
                       public base_write<Traits>,
                       public base_notification<Traits>
    {
        friend class base_tree<Traits, ptree_node<Traits> >;
    public:
        ptree_node();
        ~ptree_node() override;

        [[nodiscard]] bool is_root() const;

    private:
        ptree_node(typename Traits::node_ptr node);

    private:
        bool is_root_ = false;
        typename Traits::node_ptr node_;
    };

} // namespace

#endif // PTREE_NODE_H

#include "ptree_node.hpp"
