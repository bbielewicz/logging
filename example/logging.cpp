#include <string>
#include <thread>
#include <vector>

#include "logging/log.h"
#include "logging/manager.h"
#include "logging/sink_console.h"

void print(const unsigned amount)
{
  for (unsigned i{0}; i < amount; ++i) {
    logging::info("COUT") << "iteration: " << std::to_string(i);
  }
}

int main()
{
  logging::manager::instance().attach_logger(logging::logger::type::asynchronous);
  std::string schema{"[%TimeStamp%] [thread-%Thread%] [%Location%] [%Level%] %Message%"};
  auto sink = std::make_shared<logging::sink_console>("COUT", logging::level::debug, schema);
  logging::manager::instance().register_sink(sink);

  std::vector<std::thread> threads;
  const unsigned number_of_threads= 4;
  const unsigned number_of_log_entries = 100000;

  for (unsigned i{0}; i < number_of_threads; ++i) {
    threads.emplace_back(std::move(std::thread{print, number_of_log_entries}));
  }
  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  logging::manager::instance().destroy();
}
