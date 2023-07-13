project(
  'logging',
  'cpp',
  default_options : [
    'buildtype=release',
    'cpp_std=c++20',
    'warning_level=1'
  ]
)

logging_headers = [
  'include/logging/formatter.h',
  'include/logging/level.h',
  'include/logging/log.h',
  'include/logging/logger.h',
  'include/logging/logger_asynchronous.h',
  'include/logging/logger_synchronous.h',
  'include/logging/manager.h',
  'include/logging/record.h',
  'include/logging/sink.h',
  'include/logging/sink_console.h',
  'include/logging/sink_file.h'
]

install_headers(
  logging_headers,
  subdir : 'logging'
)

logging_include_directories = include_directories('include')

logging_sources = [
  'src/formatter.cpp',
  'src/level.cpp',
  'src/log.cpp',
  'src/logger.cpp',
  'src/logger_asynchronous.cpp',
  'src/logger_synchronous.cpp',
  'src/manager.cpp',
  'src/sink.cpp',
  'src/sink_console.cpp',
  'src/sink_file.cpp'
]

logging_library = static_library(
  'logging',
  logging_sources,
  include_directories : logging_include_directories
)

logging_dep = declare_dependency(
  include_directories : logging_include_directories,
  link_with : logging_library
)

logging_example = executable(
  'logging',
  'example/logging.cpp',
  include_directories : logging_include_directories,
  link_with : logging_library,
  dependencies : dependency('threads')
)
