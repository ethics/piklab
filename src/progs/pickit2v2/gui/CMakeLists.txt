include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pickit2v2ui_STAT_SRCS
   pickit2v2_group_ui.cpp
)
automoc(${pickit2v2ui_STAT_SRCS})
add_library(pickit2v2ui STATIC ${pickit2v2ui_STAT_SRCS})
