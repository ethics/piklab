include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(proglistnoui_STAT_SRCS
   device_info.cpp
   prog_list.cpp
   prog_list_noui.cpp
)
automoc(${proglistnoui_STAT_SRCS})
add_library(proglistnoui STATIC ${proglistnoui_STAT_SRCS})

if(NOT QT_ONLY)
  SET(proglistui_STAT_SRCS
    device_info.cpp
    prog_list.cpp
    prog_list_ui.cpp
  )
  automoc(${proglistui_STAT_SRCS})
  add_library(proglistui STATIC ${proglistui_STAT_SRCS})
endif(NOT QT_ONLY)
