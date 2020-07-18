include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(mem24base_STAT_SRCS
   mem24.cpp
)

automoc(${mem24base_STAT_SRCS})

add_library(mem24base STATIC ${mem24base_STAT_SRCS})
