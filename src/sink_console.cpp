#include "logging/sink_console.h"

namespace logging {

sink_console::sink_console(const std::string& name, const logging::level level,
                           const std::string& schema, std::ostream& output)
  : sink(name, level, schema), output_{output}
{
}

void sink_console::write(const logging::record& record)
{
  if (formatter_) {
    output_ << formatter_->format(record);
  }
}

}
