include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(gpsim_STAT_SRCS
   gpsim.cpp
   gpsim_debug.cpp
)
automoc(${gpsim_STAT_SRCS})
add_library(gpsim STATIC ${gpsim_STAT_SRCS})
