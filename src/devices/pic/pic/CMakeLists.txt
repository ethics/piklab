include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pic_STAT_SRCS
   pic_memory.cpp
   pic_group.cpp
)
automoc(${pic_STAT_SRCS})
add_library(pic STATIC ${pic_STAT_SRCS})
