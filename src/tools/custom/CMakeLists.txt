include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(customtool_STAT_SRCS
   custom.cpp
   custom_process.cpp
)

automoc(${customtool_STAT_SRCS})

add_library(customtool STATIC ${customtool_STAT_SRCS})
