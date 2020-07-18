include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(toollist_STAT_SRCS
   tool_list.cpp
   tools_config_widget.cpp
   compile_config.cpp
   compile_manager.cpp
   compile_process.cpp
)
automoc(${toollist_STAT_SRCS})
add_library(toollist STATIC ${toollist_STAT_SRCS})
