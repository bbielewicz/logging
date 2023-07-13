#include "logging/sink.h"

namespace logging {

sink::sink(const std::string& name, const logging::level level,
           const std::string& schema)
  : name_{name}, level_{static_cast<unsigned>(level)}
{
  formatter_ = std::make_unique<logging::formatter>(schema);
}

std::string sink::name() &&
{
  return name_;
}

const std::string& sink::name() const&
{
  return name_;
}

logging::level sink::level() const
{
  return static_cast<logging::level>(level_.load(std::memory_order_acquire));
}

void sink::level(const logging::level level)
{
  level_.store(static_cast<unsigned>(level), std::memory_order_release);
}

bool sink::check_level(const logging::level level) const
{
  unsigned current = level_.load(std::memory_order_acquire);
  return current != static_cast<unsigned>(logging::level::none) &&
         current <= static_cast<unsigned>(level);
}

}
