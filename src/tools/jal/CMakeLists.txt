add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(jal_STAT_SRCS
   jal_compile.cpp
   jal_config.cpp
   jal.cpp
   jal_generator.cpp
)

automoc(${jal_STAT_SRCS})

add_library(jal STATIC ${jal_STAT_SRCS})
