include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(toolui_STAT_SRCS
   tool_group_ui.cpp
   toolchain_config_widget.cpp
   tool_config_widget.cpp
   toolchain_config_center.cpp
)

automoc(${toolui_STAT_SRCS})

add_library(toolui STATIC ${toolui_STAT_SRCS})
