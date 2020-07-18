add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(gputils_STAT_SRCS
   gputils_compile.cpp
   gputils_config.cpp
   gputils.cpp
   gputils_generator.cpp
)

automoc(${gputils_STAT_SRCS})

add_library(gputils STATIC ${gputils_STAT_SRCS})
