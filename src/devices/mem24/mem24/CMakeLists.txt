include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(mem24_STAT_SRCS
   mem24_memory.cpp
   mem24_group.cpp
)
automoc(${mem24_STAT_SRCS})
add_library(mem24 STATIC ${mem24_STAT_SRCS})
