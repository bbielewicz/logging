#include "logging/formatter.h"

#include <chrono>
#include <iomanip>
#include <string_view>

namespace logging {

formatter::formatter(const std::string& schema)
  : callbacks_{initialize_callbacks()}
{
  parse_schema(schema);
}

std::string formatter::format(const logging::record& record) const
{
  std::string output{};
  output.reserve(512);
  for (auto& item : schema_) {
    if (std::holds_alternative<std::string>(item)) {
      output += std::get<std::string>(item);
    } else {
      output += std::get<formatter::callback>(item)(record);
    }
  }
  output += '\n';
  return output;
}

void formatter::parse_schema(const std::string& schema)
{
  std::size_t previous{0};
  std::size_t found = schema.find_first_of('%');
  while (found != std::string::npos) {
    std::size_t next = schema.find_first_of('%', found + 1);
    if (next == std::string::npos) {
      schema_.push_back(schema.substr(previous));
      break;
    }
    if (!callbacks_.empty()) {
      auto search = callbacks_.find(schema.substr(found, next - found + 1));
      if (search != callbacks_.end()) {
        schema_.push_back(schema.substr(previous, found - previous));
        schema_.push_back(search->second);
        previous = next + 1;
      } else {
        schema_.push_back(schema.substr(previous, found - previous + 1));
        previous = found + 1;
      }
    }
    found = schema.find_first_of('%', previous);
  }
  if (found == std::string::npos) {
    schema_.push_back(schema.substr(previous));
  }
}

std::string formatter::filename(const logging::record& record) const
{
  std::string_view filename{record.location.file_name()};
  return std::string{filename.substr(filename.find_last_of("/\\") + 1)};
}

std::string formatter::function(const logging::record& record) const
{
  std::string_view function{record.location.function_name()};
  auto function_end{function.find_first_of('(')};
  auto function_begin{function.find_last_of(" :", function_end) + 1};
  auto function_length{function_end - function_begin};
  return std::string{function.substr(function_begin, function_length)};
}

std::string formatter::level(const logging::record& record) const
{
  return logging::level_to_string(record.level);
}

std::string formatter::line(const logging::record& record) const
{
  return std::to_string(record.location.line());
}

std::string formatter::location(const logging::record& record) const
{
  return filename(record) + ':' + line(record) + ':' + function(record);
}

std::string formatter::message(const logging::record& record) const
{
  return record.message;
}

std::string formatter::thread(const logging::record& record) const
{
  return std::to_string(record.thread_id);
}

std::string formatter::timestamp(const logging::record& record) const
{
  namespace chrono = std::chrono;
  char datetime[24];
  auto time{chrono::system_clock::to_time_t(record.time_point)};
  std::strftime(datetime, sizeof(datetime), "%F %T.", std::gmtime(&time));
  std::string output{datetime};
  std::string microseconds{std::to_string((
    chrono::time_point_cast<chrono::microseconds>(record.time_point) -
    chrono::time_point_cast<chrono::seconds>(record.time_point)).count())};
  output.append(6 - microseconds.length(), '0');
  output += microseconds;
  return output;
}

const std::unordered_map<std::string, formatter::callback>
formatter::initialize_callbacks() const
{
  using namespace std::placeholders;
  return {
    {"%FileName%" , std::bind(&formatter::filename , this, _1)},
    {"%Function%" , std::bind(&formatter::function , this, _1)},
    {"%Level%"    , std::bind(&formatter::level    , this, _1)},
    {"%Line%"     , std::bind(&formatter::line     , this, _1)},
    {"%Location%" , std::bind(&formatter::location , this, _1)},
    {"%Message%"  , std::bind(&formatter::message  , this, _1)},
    {"%Thread%"   , std::bind(&formatter::thread   , this, _1)},
    {"%TimeStamp%", std::bind(&formatter::timestamp, this, _1)},
  };
}

}
