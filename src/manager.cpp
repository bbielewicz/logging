#include "logging/manager.h"

namespace logging {

std::atomic<manager*> manager::instance_;

manager& manager::instance()
{
  manager* instance = instance_.load(std::memory_order_acquire);
  if (instance == nullptr) {
    manager* ptr = new manager();
    if (!instance_.compare_exchange_strong(instance, ptr,
      std::memory_order_release, std::memory_order_relaxed)) {
      delete ptr;
    } else {
      return *ptr;
    }
  }
  return *instance;
}

void manager::destroy()
{
  manager* instance = instance_.load(std::memory_order_acquire);
  if (instance != nullptr) {
    if (instance->logger_) {
      instance->logger_->stop();
    }
    manager* ptr = nullptr;
    if (instance_.compare_exchange_strong(instance, ptr,
      std::memory_order_release, std::memory_order_relaxed)) {
      delete instance;
    }
  }
}

void manager::attach_logger(logging::logger::type type)
{
  if (logger_ == nullptr) {
    logger_ = std::move(logger::create(type));
  }
}

const std::unique_ptr<logging::logger>& manager::logger() const
{
  return logger_;
}

void manager::register_sink(std::shared_ptr<logging::sink> sink)
{
  if (sink) {
    std::lock_guard lock(mutex_);
    sinks_.emplace(sink->name(), sink);
  }
}

void manager::deregister_sink(std::shared_ptr<logging::sink> sink)
{
  if (sink) {
    std::lock_guard lock(mutex_);
    sinks_.erase(sink->name());
  }
}

void manager::deregister_sink(const std::string& sink_name)
{
  std::lock_guard lock(mutex_);
  sinks_.erase(sink_name);
}

std::shared_ptr<logging::sink> manager::find_sink(const std::string& sink_name)
{
  std::shared_lock lock(mutex_);
  if (!sinks_.empty()) {
    auto search = sinks_.find(sink_name);
    if (search != sinks_.end()) {
      return search->second;
    }
  }
  return nullptr;
}

}
