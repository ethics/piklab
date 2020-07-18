include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(icd2data_STAT_SRCS
   icd2_data.cpp
)
add_library(icd2data STATIC ${icd2data_STAT_SRCS})
