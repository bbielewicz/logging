#ifndef LOGGING_SINK_H
#define LOGGING_SINK_H

#include <atomic>
#include <memory>
#include <string>
#include "logging/formatter.h"
#include "logging/level.h"
#include "logging/record.h"

namespace logging {

class sink
{
public:
  sink(const std::string& name, const logging::level level,
       const std::string& schema);
  virtual ~sink() = default;
  virtual std::string name() && final;
  virtual const std::string& name() const& final;
  virtual void level(const logging::level level) final;
  virtual logging::level level() const final;
  virtual bool check_level(const logging::level level) const final;
  virtual void write(const logging::record& record) = 0;

protected:
  std::string name_;
  std::atomic<unsigned> level_;
  std::unique_ptr<logging::formatter> formatter_;
};

}
#endif
