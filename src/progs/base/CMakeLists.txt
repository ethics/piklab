include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(progbase_STAT_SRCS
   generic_prog.cpp
   prog_specific.cpp
   prog_config.cpp
   prog_group.cpp
   generic_debug.cpp
   hardware_config.cpp
   debug_config.cpp
)
automoc(${progbase_STAT_SRCS})
add_library(progbase STATIC ${progbase_STAT_SRCS})
