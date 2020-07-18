include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pickit3ui_STAT_SRCS
   pickit3_group_ui.cpp
)
automoc(${pickit3ui_STAT_SRCS})
add_library(pickit3ui STATIC ${pickit3ui_STAT_SRCS})
