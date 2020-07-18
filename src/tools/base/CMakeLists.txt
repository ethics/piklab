include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(toolbase_STAT_SRCS
   generic_tool.cpp
   tool_group.cpp
)
automoc(${toolbase_STAT_SRCS})
add_library(toolbase STATIC ${toolbase_STAT_SRCS})
