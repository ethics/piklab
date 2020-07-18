include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(mem24prog_STAT_SRCS
   mem24_prog.cpp
)
automoc(${mem24prog_STAT_SRCS})
add_library(mem24prog STATIC ${mem24prog_STAT_SRCS})
