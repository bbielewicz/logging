#ifndef LOGGING_LOGGER_ASYNCHRONOUS_H
#define LOGGING_LOGGER_ASYNCHRONOUS_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include "logging/logger.h"

namespace logging {

class logger_asynchronous final : public logger
{
public:
  logger_asynchronous();
  ~logger_asynchronous();
  void start() override;
  void stop() override;
  void dispatch(const logging::record& record) override;

private:
  void work();

  enum class state {
    off      = 0,
    stopping = 1,
    starting = 2,
    working  = 3
  };

  std::atomic<unsigned> state_;
  std::thread worker_;
  std::mutex mutex_;
  std::condition_variable write_condition_;
  std::queue<logging::record> records_;
};

}
#endif
