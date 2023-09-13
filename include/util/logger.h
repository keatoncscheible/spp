/******************************************************************************
 * Filename:    logger.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

class Logger {
   public:
    static Logger& instance();

    void trace(const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);

   private:
    Logger();
    std::shared_ptr<spdlog::logger> logger_;
};

#endif  // LOGGER_H
