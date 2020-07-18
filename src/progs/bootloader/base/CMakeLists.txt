include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(bootloader_STAT_SRCS
   bootloader_prog.cpp
   bootloader.cpp
)
automoc(${bootloader_STAT_SRCS})
add_library(bootloader STATIC ${bootloader_STAT_SRCS})
