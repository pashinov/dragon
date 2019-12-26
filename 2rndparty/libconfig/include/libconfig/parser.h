#ifndef LIBCONFIG_PARSER_H
#define LIBCONFIG_PARSER_H

// internal
#include "trsits.h"
#include "libconfig_error.h"
#include "libconfig/libconfig_error.h"

namespace libcfg
{
    class parser
    {
    public:
        static const_tree_node_t from_string(const std::string& str);
        static const_tree_node_t from_file(const std::string& filename);
    };

} // namespace

#endif // LIBCONFIG_PARSER_H
