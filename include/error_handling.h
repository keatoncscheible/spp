/******************************************************************************
 * Filename:    error_handling.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <exception>
#include <string>

class VideoCaptureException : public std::exception {
   public:
    explicit VideoCaptureException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }

   private:
    std::string message_;
};

class VideoProcessException : public std::exception {
   public:
    explicit VideoProcessException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }

   private:
    std::string message_;
};

class DiagnosticsException : public std::exception {
   public:
    explicit DiagnosticsException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }

   private:
    std::string message_;
};

#endif  // ERROR_HANDLING_H
