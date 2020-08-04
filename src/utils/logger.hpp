//
// Created by Alexey Pashinov on 04/08/20
//

#pragma once

// 3rdparty includes
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#define LOG_TRACE(logger, ...)      { logger->trace (__VA_ARGS__); }
#define LOG_DEBUG(logger, ...)      { logger->debug (__VA_ARGS__); }
#define LOG_INFO(logger, ...)       { logger->info  (__VA_ARGS__); }
#define LOG_WARN(logger, ...)       { logger->warn  (__VA_ARGS__); }
#define LOG_ERROR(logger, ...)      { logger->error (__VA_ARGS__); }
#define LOG_CRITICAL(logger, ...)   { logger->critical (__VA_ARGS__); }

typedef std::shared_ptr<spdlog::logger> LogHandle;

LogHandle LOGGER(const std::string& name, spdlog::level::level_enum level = spdlog::level::info,
                 const std::string& filename = std::string());

LogHandle LOGGER(const std::string& name, const std::string& filename);
