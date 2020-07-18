add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(sdcc_STAT_SRCS
   sdcc_compile.cpp
   sdcc_config.cpp
   sdcc.cpp
   sdcc_generator.cpp
)

automoc(${sdcc_STAT_SRCS})

add_library(sdcc STATIC ${sdcc_STAT_SRCS})
