#include "logging/sink_file.h"

namespace logging {

sink_file::sink_file(const std::string& name, const logging::level level,
                     const std::string& schema, const std::string& filename)
  : sink(name, level, schema)
{
  output_.open(filename);
}

void sink_file::write(const logging::record& record)
{
  if (formatter_) {
    std::string formated_message{formatter_->format(record)};
    std::lock_guard lock(mutex_);
    if (output_.is_open()) {
      output_ << formated_message;
      output_.flush();
    }
  }
}

}
