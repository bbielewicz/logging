#ifndef LOGGING_FORMATTER_H
#define LOGGING_FORMATTER_H

#include <functional>
#include <string>
#include <variant>
#include <vector>
#include "logging/record.h"

namespace logging {

class formatter final
{
public:
  explicit formatter(const std::string& schema);
  std::string format(const logging::record& record) const;

private:
  void parse_schema(const std::string& schema);
  std::string filename(const logging::record& record) const;
  std::string function(const logging::record& record) const;
  std::string level(const logging::record& record) const;
  std::string line(const logging::record& record) const;
  std::string location(const logging::record& record) const;
  std::string message(const logging::record& record) const;
  std::string thread(const logging::record& record) const;
  std::string timestamp(const logging::record& record) const;

  using callback = std::function<std::string(const logging::record&)>;
  const std::unordered_map<std::string, callback> initialize_callbacks() const;
  const std::unordered_map<std::string, callback> callbacks_;
  std::vector<std::variant<std::string, callback>> schema_;
};

}
#endif
