include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(tblbootloaderui_STAT_SRCS
   tbl_bootloader_ui.cpp
)
automoc(${tblbootloaderui_STAT_SRCS})
add_library(tblbootloaderui STATIC ${tblbootloaderui_STAT_SRCS})
