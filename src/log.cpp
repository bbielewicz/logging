#include "logging/log.h"

#include <atomic>
#include <thread>
#include <utility>
#include "logging/manager.h"
#include "logging/record.h"

namespace logging {

log::log(std::string sink_name, const logging::level level,
         const std::source_location location)
  : sink_name_{std::move(sink_name)}, level_{level}, location_{location}
{
}

log::~log()
{
  if (auto& logger{manager::instance().logger()}) {
    logger->dispatch(logging::record{
      std::move(sink_name_),
      std::chrono::system_clock::now(),
      location_,
      fetch_thread_id(),
      level_,
      stream_.str()
    });
  }
}

unsigned log::fetch_thread_id()
{
  static std::atomic<unsigned> counter{0};
  thread_local unsigned id = counter.fetch_add(1, std::memory_order_relaxed);
  return id;
}

}
