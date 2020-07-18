add_subdirectory(gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(picc_STAT_SRCS
   picc_compile.cpp
   picc_config.cpp
   picc.cpp
)

automoc(${picc_STAT_SRCS})

add_library(picc STATIC ${picc_STAT_SRCS})
