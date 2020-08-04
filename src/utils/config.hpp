//
// Created by Alexey Pashinov on 04/08/20
//

#pragma once

// system includes
#include <memory>

// project includes
#include <utils/config_st.hpp>

#define CONFIG() utils::config::get_instance().get_config()

namespace utils
{
    struct config_st;

    class config
    {
    public:
        static config& get_instance()
        {
            static config instance;
            return instance;
        }

        bool load_config(const std::string& filename) noexcept;

        const config_st* get_config() noexcept;

    private:
        config() : cfg_(std::make_unique<config_st>()) {};

        ~config() = default;

        config(const config&) = delete;

        config& operator=(const config&) = delete;

    private:
        std::unique_ptr<config_st> cfg_;
    };
}
