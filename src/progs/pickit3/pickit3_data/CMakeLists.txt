include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pickit3data_STAT_SRCS
   pickit3_data.cpp
)
add_library(pickit3data STATIC ${pickit3data_STAT_SRCS})
