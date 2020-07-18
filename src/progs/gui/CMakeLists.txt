include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(progui_STAT_SRCS
   prog_config_widget.cpp
   prog_group_ui.cpp
   hardware_config_widget.cpp
   prog_config_center.cpp
   port_selector.cpp
   debug_config_center.cpp
)
automoc(${progui_STAT_SRCS})
add_library(progui STATIC ${progui_STAT_SRCS})
