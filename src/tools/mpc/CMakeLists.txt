add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(mpc_STAT_SRCS
   mpc.cpp
   mpc_compile.cpp
   mpc_config.cpp
)

automoc(${mpc_STAT_SRCS})

add_library(mpc STATIC ${mpc_STAT_SRCS})
