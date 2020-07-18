add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pic30_STAT_SRCS
   pic30_compile.cpp
   pic30_config.cpp
   pic30.cpp
   pic30_generator.cpp
)

automoc(${pic30_STAT_SRCS})

add_library(pic30 STATIC ${pic30_STAT_SRCS})
