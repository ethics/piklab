include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(devparser_STAT_SRCS
   dev_parser.cpp
)
automoc(${devparser_STAT_SRCS})
add_library(devparser STATIC ${devparser_STAT_SRCS})
