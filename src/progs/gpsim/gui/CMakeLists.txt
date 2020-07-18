include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(gpsimui_STAT_SRCS
   gpsim_group_ui.cpp
)
automoc(${gpsimui_STAT_SRCS})
add_library(gpsimui STATIC ${gpsimui_STAT_SRCS})
