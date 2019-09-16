#include <logger/logger.hpp>

LogHandle LOGGER(const std::string& name, spdlog::level::level_enum level, const std::string& filename)
{
    auto logger = spdlog::get(name);

    if (!logger)
    {
        std::vector<spdlog::sink_ptr> sinks;
        if (filename.empty())
        {
            sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
        }
        else
        {
            sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename));
        }

        logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());

        logger->set_level(level);

        logger->flush_on(level);

        spdlog::register_logger(logger);
    }

    return logger;
}

LogHandle LOGGER(const std::string& name, const std::string& filename)
{
    return LOGGER(name, spdlog::level::info, filename);
}
