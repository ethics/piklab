include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(picprog_STAT_SRCS
   pic_prog.cpp
   pic_prog_specific.cpp
   pic_debug.cpp
)
automoc(${picprog_STAT_SRCS})
add_library(picprog STATIC ${picprog_STAT_SRCS})
