/******************************************************************************
 * Filename:    logger.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

Logger::Logger() {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console->set_pattern("%^%v%$");
    console->set_color(spdlog::level::critical, console->red_bold);
    console->set_color(spdlog::level::err, console->red);
    console->set_color(spdlog::level::warn, console->yellow);
    console->set_color(spdlog::level::info, console->green);
    console->set_color(spdlog::level::debug, console->blue);
    console->set_color(spdlog::level::trace, console->magenta);

    logger_ = std::make_shared<spdlog::logger>("console", console);
    logger_->set_level(spdlog::level::trace);
    spdlog::set_default_logger(logger_);
}

Logger& Logger::instance() {
    static Logger logger_instance;
    return logger_instance;
}

void Logger::trace(const std::string& message) { logger_->trace(message); }

void Logger::debug(const std::string& message) { logger_->debug(message); }
void Logger::info(const std::string& message) { logger_->info(message); }

void Logger::error(const std::string& message) { logger_->error(message); }

void Logger::warning(const std::string& message) { logger_->warn(message); }
void Logger::critical(const std::string& message) {
    logger_->critical(message);
}
