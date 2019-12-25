#ifndef LIBCONFIG_ERROR_H
#define LIBCONFIG_ERROR_H

// std
#include <string>
#include <exception>

namespace libcfg
{
    class parse_exception : public std::exception
    {
    public:
        enum class error_type
        {
            parse_exception = 0,
            config_exception,
            setting_exception,
            internal_error,
            unknown_error
        };
    public:
        parse_exception(const std::string& what, error_type type)
            : what_(what), type_(type) { }
        ~parse_exception() override = default;

        const char* what() const noexcept override { return what_.c_str(); }
        error_type type() const noexcept { return type_; }
    private:
        std::string what_;
        error_type type_;
    };

} // namespace

#endif // LIBCONFIG_ERROR_H
