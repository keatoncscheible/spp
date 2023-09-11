/******************************************************************************
 * Filename:    statistics.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef STATISTICS_QUEUE_H
#define STATISTICS_QUEUE_H

#include <cmath>
#include <deque>
#include <iostream>
#include <mutex>
#include <queue>  // Added missing include
#include <stdexcept>

template <typename T>
struct Statistics {
    T minimum;
    T maximum;
    double average;
    double variance;
    double standard_deviation;
};

template <typename T>
class StatisticsQueue {
   public:
    explicit StatisticsQueue(std::size_t max_size)
        : max_size_(max_size), sum_(0), sum_squared_(0), size_(0) {}

    void Push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.size() >= max_size_) {
            auto oldest_value = queue_.back();
            queue_.pop_back();
            max_heap_.pop();
            min_heap_.pop();
            sum_ -= oldest_value;
            sum_squared_ -=
                (oldest_value * oldest_value);  // Added missing semicolon
        }
        queue_.push_front(value);
        max_heap_.push(value);
        min_heap_.push(value);
        size_ = queue_.size();
        sum_ += value;
        sum_squared_ += (value * value);
    }

    Statistics<T> GetStatistics() {
        Statistics<T> statistics;  // Added type specifier <T> for Statistics

        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return statistics;  // Return an empty statistics object
        }

        auto inv_size = 1.0 / static_cast<double>(size_);
        auto average = static_cast<double>(sum_) * inv_size;
        auto average_squared = (average * average);
        auto variance = (static_cast<double>(sum_squared_) * inv_size) -
                        average_squared;  // Added underscore to sum_squared_

        statistics.minimum = min_heap_.top();
        statistics.maximum = max_heap_.top();
        statistics.average = average;
        statistics.variance = variance;
        statistics.standard_deviation = std::sqrt(variance);
        return statistics;
    }

   private:
    std::deque<T> queue_;
    std::priority_queue<T> max_heap_;
    std::priority_queue<T, std::vector<T>, std::greater<T>> min_heap_;
    T sum_;
    T sum_squared_;  // Added underscore to sum_squared_
    std::size_t size_;
    std::size_t max_size_;
    mutable std::mutex mutex_;
};

#endif  // STATISTICS_QUEUE_H
