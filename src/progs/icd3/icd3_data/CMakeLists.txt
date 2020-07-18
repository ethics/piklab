include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(icd3data_STAT_SRCS
   icd3_data.cpp
)
add_library(icd3data STATIC ${icd3data_STAT_SRCS})
