add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(jalv2_STAT_SRCS
   jalv2_compile.cpp
   jalv2_config.cpp
   jalv2.cpp
   jalv2_generator.cpp
)

automoc(${jalv2_STAT_SRCS})

add_library(jalv2 STATIC ${jalv2_STAT_SRCS})
