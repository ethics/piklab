add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(ccsc_STAT_SRCS
   ccsc.cpp
   ccsc_compile.cpp
   ccsc_config.cpp
)

automoc(${ccsc_STAT_SRCS})

add_library(ccsc STATIC ${ccsc_STAT_SRCS})
