/******************************************************************************
 * Filename:    statistics_queue.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef STATISTICS_QUEUE_H
#define STATISTICS_QUEUE_H

#include <cmath>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdexcept>

template <typename T>
class StatisticsQueue {
   public:
    explicit StatisticsQueue(std::size_t max_size)
        : max_size_(max_size),
          minimum_(0),
          maximum_(0),
          average_(0),
          standard_deviation_(0) {}

    void Push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.size() >= max_size_) {
            queue_.pop();
        }
        queue_.push(value);
    }

    void Pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            throw std::underflow_error("Queue is empty");
        }
        T front_value = queue_.front();
        queue_.pop();
    }

    T Front() const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            throw std::underflow_error("Queue is empty");
        }
        return queue_.front();
    }

    bool Empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    std::size_t Size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    void UpdateStatistics() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return;
        }

        minimum_ = queue_.front();
        maximum_ = queue_.front();

        T sum = 0;
        T sum_squares = 0;

        for (const T& value : queue_) {
            if (value < minimum_) {
                minimum_ = value;
            }
            if (value > maximum_) {
                maximum_ = value;
            }
            sum += value;
            sum_squares += (value * value);
        }
        auto inv_size = 1.0 / static_cast<double>(queue_.size());
        average_ = static_cast<double>(sum) * inv_size;
        auto average_squared = (average_ * average_);
        variance_ =
            (static_cast<double>(sum_squares) * inv_size) - average_squared;
        standard_deviation_ = std::sqrt(variance_);
    }

   private:
    std::queue<T> queue_;
    std::size_t max_size_;
    T minimum_;
    T maximum_;
    double average_;
    double variance_;
    double standard_deviation_;
    mutable std::mutex mutex_;
};

#endif  // STATISTICS_QUEUE_H
