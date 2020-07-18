include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(bootloaderui_STAT_SRCS
   bootloader_ui.cpp
)
automoc(${bootloaderui_STAT_SRCS})
add_library(bootloaderui STATIC ${bootloaderui_STAT_SRCS})
