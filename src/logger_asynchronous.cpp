#include "logging/logger_asynchronous.h"

#include <chrono>
#include "logging/manager.h"

namespace logging {

logger_asynchronous::logger_asynchronous()
  : state_{static_cast<unsigned>(state::off)}
{
  start();
}

logger_asynchronous::~logger_asynchronous()
{
  stop();
}

void logger_asynchronous::start()
{
  unsigned state = state_.load(std::memory_order_acquire);
  if (state == static_cast<unsigned>(state::working) ||
      state == static_cast<unsigned>(state::starting)) {
    return;
  }
  while (state == static_cast<unsigned>(state::stopping)) {
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    state = state_.load(std::memory_order_acquire);
  }
  if (state == static_cast<unsigned>(state::off)) {
    if (state_.compare_exchange_strong(
        state, static_cast<unsigned>(state::starting),
        std::memory_order_release, std::memory_order_relaxed)) {
      worker_ = std::move(std::thread{[this](){ work(); }});
      while(state_.load(std::memory_order_acquire) !=
            static_cast<unsigned>(state::working)) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
      }
    }
  }
}

void logger_asynchronous::stop()
{
  unsigned state = state_.load(std::memory_order_acquire);
  if (state == static_cast<unsigned>(state::off) ||
      state == static_cast<unsigned>(state::stopping)) {
    return;
  }
  while (state == static_cast<unsigned>(state::starting)) {
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    state = state_.load(std::memory_order_acquire);
  }
  if (state == static_cast<unsigned>(state::working)) {
    if (state_.compare_exchange_strong(
        state, static_cast<unsigned>(state::stopping),
        std::memory_order_release, std::memory_order_relaxed)) {
      write_condition_.notify_one();
      while(state_.load(std::memory_order_acquire) !=
            static_cast<unsigned>(state::off)) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
      }
      if (worker_.joinable()) {
        worker_.join();
      }
    }
  }
}

void logger_asynchronous::dispatch(const logging::record& record)
{
  auto state = state_.load(std::memory_order_acquire);
  if (state == static_cast<unsigned>(state::off) ||
      state == static_cast<unsigned>(state::stopping)) {
    return;
  }
  {
    std::unique_lock<std::mutex> lock(mutex_);
    records_.emplace(record);
  }
  write_condition_.notify_one();
}

void logger_asynchronous::work()
{
  state_.store(static_cast<unsigned>(state::working), std::memory_order_release);
  bool is_records_queue_empty{true};
  while(state_.load(std::memory_order_acquire) !=
        static_cast<unsigned>(state::stopping) ||
        is_records_queue_empty == false) {
    std::queue<logging::record> buffer;
    std::unique_lock<std::mutex> lock(mutex_);
    if (records_.empty()) {
      write_condition_.wait(lock);
    }
    std::swap(records_, buffer);
    lock.unlock();
    while(!buffer.empty()) {
      auto item = buffer.front();
      if (auto sink{manager::instance().find_sink(item.sink_name)}) {
        if (sink->check_level(item.level)) {
          sink->write(item);
        }
      }
      buffer.pop();
    }
    lock.lock();
    is_records_queue_empty = records_.empty();
  }
  state_.store(static_cast<unsigned>(state::off), std::memory_order_release);
}

}
