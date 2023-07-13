#include "logging/logger.h"

#include "logging/logger_asynchronous.h"
#include "logging/logger_synchronous.h"

namespace logging {

std::unique_ptr<logger> logger::create(logger::type type)
{
  if (type == logger::type::asynchronous) {
    return std::make_unique<logger_asynchronous>();
  } else if (type == logger::type::synchronous) {
    return std::make_unique<logger_synchronous>();
  }
  return nullptr;
}

}
