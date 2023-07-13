#ifndef LOGGING_SINK_FILE_H
#define LOGGING_SINK_FILE_H

#include <fstream>
#include <mutex>
#include "logging/sink.h"

namespace logging {

class sink_file final : public sink
{
public:
  sink_file(const std::string& name, const logging::level level,
            const std::string& schema, const std::string& filename);
  void write(const logging::record& record) override;

private:
  std::mutex mutex_;
  std::ofstream output_;
};

}
#endif
