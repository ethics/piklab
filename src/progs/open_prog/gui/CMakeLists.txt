include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(opui_STAT_SRCS
   op_group_ui.cpp
)
automoc(${opui_STAT_SRCS})
add_library(opui STATIC ${opui_STAT_SRCS})
