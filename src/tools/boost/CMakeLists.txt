add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(boost_STAT_SRCS
   boostc.cpp
   boostc_compile.cpp
   boost_config.cpp
   boost.cpp
   boostcpp.cpp
   boostbasic.cpp
   boost_generator.cpp
)
automoc(${boost_STAT_SRCS})
add_library(boost STATIC ${boost_STAT_SRCS})
