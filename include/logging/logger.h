#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <memory>
#include "logging/record.h"

namespace logging {

class logger
{
public:
  enum class type {
    asynchronous = 0,
    synchronous  = 1
  };
  static std::unique_ptr<logger> create(logger::type type);
  virtual ~logger() = default;
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void dispatch(const logging::record& record) = 0;
};

}
#endif
