include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(progmanager_STAT_SRCS
   breakpoint.cpp
   debug_manager.cpp
   prog_manager.cpp
)
automoc(${progmanager_STAT_SRCS})
add_library(progmanager STATIC ${progmanager_STAT_SRCS})
