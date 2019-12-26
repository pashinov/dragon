// internal
#include <Libconfig>
#include <PropertyTree>

// libconfig++
#include <libconfig.h++>

// std
#include <cassert>

using namespace libcfg;

using tree_node_t = ptree::ptree_node<libcfg::tree_types>;
using SettingType = libconfig::Setting::Type;
using libconfig_node_t = libconfig::Setting;

namespace
{
    void preordered_node(const libconfig_node_t& node, tree_node_t& cfg_tree);
    void preordered_arrary(const libconfig_node_t& node, tree_node_t& tree);
    void preordered_list(const libconfig_node_t& node, tree_node_t& tree);

    // обход листа
    void preordered_list(const libconfig_node_t& node, tree_node_t& cfg)
    {
        int index = 0;
        for(auto it = node.begin(); it != node.end(); ++it)
        {
            tree_types::key_t key = std::to_string(index);
            switch(it->getType())
            {
            case SettingType::TypeInt:
                if (!cfg[key].set_value(it->operator int()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeFloat:
                if (!cfg[key].set_value(it->operator double()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeInt64:
                if (!cfg[key].set_value(it->operator long long()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeString:
                if (!cfg[key].set_value(std::string(it->operator const char *())))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeBoolean:
                if (!cfg[key].set_value(it->operator bool()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeList:
            {
                auto new_list = cfg.child(key);
                preordered_list(*it, new_list);
                break;
            }
            case SettingType::TypeNone:
                if (!cfg[key].set_value(tree_types::value_t()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            case SettingType::TypeGroup:
            {
                auto new_node = cfg.child(key);
                preordered_node(*it, new_node);
                break;
            }
            case SettingType::TypeArray:
            {
                auto new_array = cfg.child(it->getName());
                preordered_arrary(*it, new_array);
                break;
            }
            }
            ++index;
        }
    }

    // обход массива
    void preordered_arrary(const libconfig::Setting& node, tree_node_t& tree)
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

    // обход дерева
    void preordered_node(const libconfig_node_t& node, tree_node_t& cfg)
    {
        for(auto it = node.begin(); it != node.end(); ++it)
        {
            tree_types::key_t name = it->getName();
            switch (it->getType())
            {
            case SettingType::TypeInt:
                if (!cfg[name].set_value(it->operator int()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeFloat:
                if (!cfg[name].set_value(it->operator double()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeInt64:
                if (!cfg[name].set_value(it->operator long long()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeString:
                if (!cfg[name].set_value(std::string(it->operator const char*())))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeBoolean:
                if (!cfg[name].set_value(it->operator bool()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::internal_error);
                break;
            case SettingType::TypeList:
            {
                auto list_node = cfg.child(it->getName());
                preordered_list(*it, list_node);
                break;
            }
            case SettingType::TypeArray:
            {
                auto array_node = cfg.child(it->getName());
                preordered_arrary(*it, array_node);
                break;
            }
            case SettingType::TypeGroup:
            {
                auto new_node = cfg.child(it->getName());
                preordered_node(*it, new_node);
                break;
            }
            case SettingType::TypeNone:
                // TODO: using std::monostate
                if (!cfg[name].set_value(tree_types::value_t()))
                    throw parse_exception("internal error in set value",
                                          parse_exception::error_type::unknown_error);
                break;
            }
        }
    }

} // namespace

const_tree_node_t parser::from_string(const std::string& str)
{
    assert(!str.empty());

    libconfig::Config libcfg;
    try
    {
        libcfg.readString(str);
    }
    catch(const libconfig::ParseException& ex)
    {
        std::cout << ex.what() << std::endl;
        throw parse_exception(ex.what(), parse_exception::error_type::parse_exception);
    }
    catch(const libconfig::ConfigException& ex)
    {
        throw parse_exception(ex.what(), parse_exception::error_type::config_exception);
        std::cout << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "unknown error" << std::endl;
        throw parse_exception("unknown error in libconfig parser",
                              parse_exception::error_type::unknown_error);
    }

    tree_node_t cfg;
    try
    {
        preordered_node(libcfg.getRoot(), cfg);
    }
    catch(const parse_exception& ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw parse_exception("unknown error in config tree",
                              parse_exception::error_type::unknown_error);
    }

    return const_tree_node_t(cfg);
}

const_tree_node_t parser::from_file(const std::string& filename)
{
    assert(!filename.empty());

    libconfig::Config libcfg;
    try
    {
        libcfg.readFile(filename.c_str());
    }
    catch(const libconfig::ParseException& ex)
    {
        std::cout << ex.what() << std::endl;
        throw parse_exception(ex.what(), parse_exception::error_type::parse_exception);
    }
    catch(const libconfig::ConfigException& ex)
    {
        throw parse_exception(ex.what(), parse_exception::error_type::config_exception);
        std::cout << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "unknown error" << std::endl;
        throw parse_exception("unknown error in libconfig parser",
                              parse_exception::error_type::unknown_error);
    }

    tree_node_t cfg;
    try
    {
        preordered_node(libcfg.getRoot(), cfg);
    }
    catch(const parse_exception& ex)
    {
        throw ex;
    }
    catch(...)
    {
        throw parse_exception("unknown error in config tree",
                              parse_exception::error_type::unknown_error);
    }

    return const_tree_node_t(cfg);
}
