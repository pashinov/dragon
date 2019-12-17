#ifndef LIBCONFIG_TRAITS_H
#define LIBCONFIG_TRAITS_H

// ptree
#include <ConstPtree>

// std
#include <variant>

namespace libcfg
{
    using tree_types = ptree::traits<std::string, std::variant<int, float, long long,
                            std::string, std::monostate> >;
    using const_tree_t = ptree::ptree_const_node<tree_types>;

} // namespace

#endif // TRAITS_H
