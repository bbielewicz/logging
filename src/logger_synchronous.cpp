#include "logging/logger_synchronous.h"

#include "logging/manager.h"

namespace logging {

logger_synchronous::logger_synchronous()
  : is_working_{false}
{
  start();
}

logger_synchronous::~logger_synchronous()
{
  stop();
}

void logger_synchronous::start()
{
  is_working_.store(true, std::memory_order_release);
}

void logger_synchronous::stop()
{
  is_working_.store(false, std::memory_order_release);
}

void logger_synchronous::dispatch(const logging::record& record)
{
  if (is_working_.load(std::memory_order_acquire)) {
    if (auto sink{manager::instance().find_sink(record.sink_name)}) {
      if (sink->check_level(record.level)) {
        sink->write(record);
      }
    }
  }
}

}
