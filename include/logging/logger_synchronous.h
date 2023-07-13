#ifndef LOGGING_LOGGER_SYNCHRONOUS_H
#define LOGGING_LOGGER_SYNCHRONOUS_H

#include "atomic"
#include "logging/logger.h"

namespace logging {

class logger_synchronous final : public logger
{
public:
  logger_synchronous();
  ~logger_synchronous();
  void start() override;
  void stop() override;
  void dispatch(const logging::record& record) override;

private:
  std::atomic<bool> is_working_;
};

}
#endif
