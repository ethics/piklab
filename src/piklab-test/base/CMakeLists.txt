include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(test_STAT_SRCS
   main_test.cpp
   device_test.cpp
   generator_check.cpp
)

automoc(${test_STAT_SRCS})

add_library(test STATIC ${test_STAT_SRCS})
