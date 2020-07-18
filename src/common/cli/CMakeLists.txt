include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(cli_STAT_SRCS
   cli_interactive.cpp
   cli_log.cpp
   cli_main.cpp
)
automoc(${cli_STAT_SRCS})
add_library(cli STATIC ${cli_STAT_SRCS})
