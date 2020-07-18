include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pickit2ui_STAT_SRCS
   pickit2_group_ui.cpp
)
automoc(${pickit2ui_STAT_SRCS})
add_library(pickit2ui STATIC ${pickit2ui_STAT_SRCS})
