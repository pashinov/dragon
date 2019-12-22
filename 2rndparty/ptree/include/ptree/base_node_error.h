#ifndef BASE_NODE_ERROR_H
#define BASE_NODE_ERROR_H

// std
#include <string>
#include <exception>

namespace ptree
{
    class base_node_error : public std::exception
    {
    public:
        enum class error_type : std::uint64_t
        {
            value_not_exist,
            children_not_exist
        };
    public:
        base_node_error(base_node_error::error_type type, std::string&& description)
            : type_(type), description_(description) { }

        base_node_error(base_node_error::error_type type, const std::string& description)
                : type_(type), description_(description) { }

        [[nodiscard]] const char* what() const noexcept override { return description_.c_str(); }
        [[nodiscard]] error_type type() const { return type_; }

    private:
        error_type type_;
        std::string description_;
    };

} // namespace

#endif // BASE_NODE_ERROR_H
