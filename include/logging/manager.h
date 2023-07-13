#ifndef LOGGING_MANAGER_H
#define LOGGING_MANAGER_H

#include <atomic>
#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include "logging/logger.h"
#include "logging/sink.h"

namespace logging {

class manager final
{
public:
  static manager& instance();
  static void destroy();
  void attach_logger(logging::logger::type type);
  const std::unique_ptr<logging::logger>& logger() const;
  void register_sink(std::shared_ptr<logging::sink> sink);
  void deregister_sink(std::shared_ptr<logging::sink> sink);
  void deregister_sink(const std::string& sink_name);
  std::shared_ptr<logging::sink> find_sink(const std::string& sink_name);

private:
  manager() = default;
  manager(const manager&) = delete;
  manager& operator=(const manager&) = delete;

  static std::atomic<manager*> instance_;
  std::unique_ptr<logging::logger> logger_;
  std::shared_mutex mutex_;
  std::unordered_map<std::string, std::shared_ptr<logging::sink>> sinks_;
};

}
#endif
