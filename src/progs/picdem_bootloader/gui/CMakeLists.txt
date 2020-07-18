include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(picdembootloaderui_STAT_SRCS
   picdem_bootloader_ui.cpp
)
automoc(${picdembootloaderui_STAT_SRCS})
add_library(picdembootloaderui STATIC ${picdembootloaderui_STAT_SRCS})
