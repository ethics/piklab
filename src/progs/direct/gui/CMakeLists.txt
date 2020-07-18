include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(directui_STAT_SRCS
   direct_config_widget.cpp
)
automoc(${directui_STAT_SRCS})
add_library(directui STATIC ${directui_STAT_SRCS})
