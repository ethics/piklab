add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(c18_STAT_SRCS
   c18_compile.cpp
   c18_config.cpp
   c18.cpp
)

automoc(${c18_STAT_SRCS})

add_library(c18 STATIC ${c18_STAT_SRCS})
