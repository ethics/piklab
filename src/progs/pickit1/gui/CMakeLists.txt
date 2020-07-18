include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pickit1ui_STAT_SRCS
   pickit1_group_ui.cpp
)
automoc(${pickit1ui_STAT_SRCS})
add_library(pickit1ui STATIC ${pickit1ui_STAT_SRCS})
