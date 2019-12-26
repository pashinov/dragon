#ifndef LIBCONFIG_TRAITS_H
#define LIBCONFIG_TRAITS_H

// ptree
#include <ConstPtree>

// std
#include <variant>

namespace libcfg
{
    using tree_types = ptree::traits<std::string, std::variant<int, float, double, long long, bool,
                            std::string> >;
    using const_tree_node_t = ptree::ptree_const_node<tree_types>;

} // namespace

#endif // TRAITS_H
