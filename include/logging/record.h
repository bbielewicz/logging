#ifndef LOGGING_RECORD_H
#define LOGGING_RECORD_H

#include <chrono>
#include <string>
#include <source_location>
#include "logging/level.h"

namespace logging {

struct record
{
  std::string sink_name;
  std::chrono::system_clock::time_point time_point;
  std::source_location location;
  unsigned thread_id;
  logging::level level;
  std::string message;
};

}
#endif
