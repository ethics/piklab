include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(cc5xui_STAT_SRCS
   cc5x_ui.cpp
)

automoc(${cc5xui_STAT_SRCS})

add_library(cc5xui STATIC ${cc5xui_STAT_SRCS})
