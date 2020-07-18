include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pickit2bootloaderui_STAT_SRCS
   pickit2_bootloader_ui.cpp
)
automoc(${pickit2bootloaderui_STAT_SRCS})
add_library(pickit2bootloaderui STATIC ${pickit2bootloaderui_STAT_SRCS})
