// internal
#include <Libconfig>
#include <PropertyTree>

// libconfig++
#include <libconfig.h++>

using namespace libcfg;

using tree_t = ptree::ptree_node<libcfg::tree_types>;
using SettingType = libconfig::Setting::Type;
using libconfig_node_t = libconfig::Setting;

namespace
{
    void make_node(const libconfig_node_t& node, tree_t& cfg_tree);
    void arrary_parse(const libconfig_node_t& node, tree_t& tree);
    void list_parse(const libconfig_node_t& node, tree_t& tree);

    void list_parse(const libconfig::Setting& node, tree_t& tree)
    {
        int index = 0;
        for(auto lit = node.begin(); lit != node.end(); ++lit)
        {
            std::string key = std::to_string(index);
            switch(lit->getType())
            {
            case SettingType::TypeInt:
            {
                if (!tree[key].set_value(lit->operator int()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            }
            case SettingType::TypeFloat:
            {
                if (!tree[key].set_value(lit->operator double()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            }
            case SettingType::TypeInt64:
                if (!tree[key].set_value(lit->operator long long()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeString:
                if (!tree[key].set_value(std::string(lit->operator const char *())))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeBoolean:
                if (!tree[key].set_value(lit->operator bool()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeList:
            {
                auto new_node = tree.child(key);
                list_parse(*lit, new_node);
                break;
            }
            case SettingType::TypeNone:
            {
                if (!tree[key].set_value(tree_types::value_t()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            }
            case SettingType::TypeGroup:
            {
                auto new_node = tree.child(key);
                make_node(*lit, new_node);
                break;
            }
            case SettingType::TypeArray:
            {
                auto new_node = tree.child(lit->getName());
                arrary_parse(*lit, new_node);
                break;
            }
            }
            ++index;
        }
    }

    void arrary_parse(const libconfig::Setting& node, tree_t& tree)
    {
        int index = 0;
        for(auto it = node.begin(); it != node.end(); ++it)
        {
            std::string name = std::to_string(index);
            auto new_node = tree[name];
            switch (it->getType())
            {
            case SettingType::TypeInt:
            {
                if (!new_node.set_value(it->operator int()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            }
            case SettingType::TypeFloat:
            {
                if (!new_node.set_value(it->operator double()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            }
            case SettingType::TypeInt64:
                if (!new_node.set_value(it->operator long long()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeString:
                if (!new_node.set_value(std::string(it->operator const char*())))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeBoolean:
                if (!new_node.set_value(it->operator bool()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeList:
            case SettingType::TypeArray:
            case SettingType::TypeGroup:
            case SettingType::TypeNone:
                break;
            }
            ++index;
        }
    }

    void make_node(const libconfig_node_t &node, tree_t& cfg_tree)
    {
        for(auto it = node.begin(); it != node.end(); ++it)
        {
            const char* name = it->getName();
            switch (it->getType())
            {
            case SettingType::TypeInt:
            {
                if (!cfg_tree[name].set_value(it->operator int()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            }
            case SettingType::TypeFloat:
            {
                if (!cfg_tree[name].set_value(it->operator double()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            }
            case SettingType::TypeInt64:
                if (!cfg_tree[name].set_value(it->operator long long()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeString:
                if (!cfg_tree[name].set_value(std::string(it->operator const char*())))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeBoolean:
                if (!cfg_tree[name].set_value(it->operator bool()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeList:
            {
                auto new_node = cfg_tree.child(it->getName());
                list_parse(*it, new_node);
                break;
            }
            case SettingType::TypeArray:
            {
                auto new_node = cfg_tree.child(it->getName());
                arrary_parse(*it, new_node);
                break;
            }
            case SettingType::TypeGroup:
            {
                auto new_node = cfg_tree.child(it->getName());
                make_node(*it, new_node);
                break;
            }
            case SettingType::TypeNone:
            {
                tree_types::value_t v;
                if (!cfg_tree[name].set_value(v))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            }
            }
        }
    }

} // namespace

const_tree_t parser::from_string(const std::string& str)
{
    libconfig::Config cfg;
    try
    {
        cfg.readString(str);
    }
    catch(const libconfig::ParseException& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch(const libconfig::ConfigException& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "unknown error" << std::endl;
    }

    tree_t cfg_tree;
    const libconfig::Setting& root = cfg.getRoot();
    make_node(root, cfg_tree);

    return const_tree_t(cfg_tree);
}

const_tree_t parser::from_file(const std::string& filename)
{
    return const_tree_t();
}
