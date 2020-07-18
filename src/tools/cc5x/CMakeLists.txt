add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(cc5x_STAT_SRCS
   cc5x.cpp
   cc5x_compile.cpp
   cc5x_config.cpp
)

automoc(${cc5x_STAT_SRCS})

add_library(cc5x STATIC ${cc5x_STAT_SRCS})
