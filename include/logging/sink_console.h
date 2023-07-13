#ifndef LOGGING_SINK_CONSOLE_H
#define LOGGING_SINK_CONSOLE_H

#include <iostream>
#include "logging/sink.h"

namespace logging {

class sink_console final : public sink
{
public:
  sink_console(const std::string& name, const logging::level level,
               const std::string& schema, std::ostream& output = std::cout);
  void write(const logging::record& record) override;

private:
  std::ostream& output_;
};

}
#endif
