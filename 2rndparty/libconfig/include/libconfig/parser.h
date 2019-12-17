#ifndef LIBCONFIG_PARSER_H
#define LIBCONFIG_PARSER_H

// internal
#include "trsits.h"

namespace libcfg
{
    class parser
    {
    public:
        static const_tree_t parse_from_str(const std::string& str);
        static const_tree_t parse_from_file(const std::string& filename);
    };

} // namespace

#endif // LIBCONFIG_PARSER_H
