include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pickit2v2_STAT_SRCS
   pickit2v2.cpp
   pickit2v2_data.cpp
   pickit2v2_prog.cpp
)
automoc(${pickit2v2_STAT_SRCS})
add_library(pickit2v2 STATIC ${pickit2v2_STAT_SRCS})
