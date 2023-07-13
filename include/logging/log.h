#ifndef LOGGING_LOG_H
#define LOGGING_LOG_H

#include <sstream>
#include <string>
#include <source_location>
#include "logging/level.h"

namespace logging {

class log final
{
public:
  log(std::string sink_name, const logging::level level,
      const std::source_location location = std::source_location::current());
  ~log();

  template<typename T>
  log& operator<<(const T& value) {
    stream_ << value;
    return *this;
  }

private:
  log(const log&) = delete;
  log& operator=(const log&) = delete;
  unsigned fetch_thread_id();

  std::ostringstream stream_;
  std::string sink_name_;
  logging::level level_;
  std::source_location location_;
};

#define debug(sink)    log(sink, logging::level::debug)
#define info(sink)     log(sink, logging::level::info)
#define warning(sink)  log(sink, logging::level::warning)
#define error(sink)    log(sink, logging::level::error)
#define critical(sink) log(sink, logging::level::critical)

}
#endif
