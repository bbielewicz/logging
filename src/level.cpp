#include "logging/level.h"

#include <unordered_map>

namespace logging {

const std::unordered_map<logging::level, std::string> level_string_map{
  {logging::level::debug   , "debug"},
  {logging::level::info    , "info"},
  {logging::level::warning , "warning"},
  {logging::level::error   , "error"},
  {logging::level::critical, "critical"}
};

std::string level_to_string(const logging::level level)
{
  auto search = level_string_map.find(level);
  if (search != level_string_map.end()) {
    return search->second;
  }
  return std::string{"none"};
}

}
