// internal
#include <Libconfig>
#include <PropertyTree>

// libconfig++
#include <libconfig.h++>

using namespace libcfg;

using tree_t = ptree::ptree_node<libcfg::tree_types>;

namespace
{
    void make_node(const libconfig::Setting& node)
    {
    }

} // namespace

const_tree_t parser::parse_from_str(const std::string& str)
{
    libconfig::Config cfg;
    cfg.readString(str);

    tree_t cfg_tree;
    const libconfig::Setting& root = cfg.getRoot();
    for(auto it = root.begin(); it != root.end(); it++)
    {
        const char* name = it->getName();
        using SettingType = libconfig::Setting::Type;
        switch (it->getType())
        {
        case SettingType::TypeInt:
            cfg_tree[name].set_value(int(*it));
            break;
        case SettingType::TypeFloat:
        {
            cfg_tree[name].set_value(float(*it));
            float f = float(*it);
            break;
        }
        case SettingType::TypeInt64:
            cfg_tree[name].set_value(int(*it));
            break;
        case SettingType::TypeString:
            cfg_tree[name].set_value(std::string(*it));
            break;
        case SettingType::TypeBoolean:
            cfg_tree[name].set_value(bool(*it));
            break;
        case SettingType::TypeList:
        case SettingType::TypeArray:
        case SettingType::TypeGroup:
        case SettingType::TypeNone:
            break;
        }
    }

    return const_tree_t(cfg_tree);
}

const_tree_t parser::parse_from_file(const std::string& filename)
{
    return const_tree_t();
}
