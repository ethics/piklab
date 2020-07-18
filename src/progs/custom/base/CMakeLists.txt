include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(customprog_STAT_SRCS
   custom_prog.cpp
)
automoc(${customprog_STAT_SRCS})
add_library(customprog STATIC ${customprog_STAT_SRCS})
