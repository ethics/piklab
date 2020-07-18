include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(devicelistnoui_STAT_SRCS
   device_list.cpp
   device_list_noui.cpp
)
automoc(${devicelistnoui_STAT_SRCS})
add_library(devicelistnoui STATIC ${devicelistnoui_STAT_SRCS})

if(NOT QT_ONLY)
  SET(devicelistui_STAT_SRCS
    device_list.cpp
    device_list_ui.cpp
  )
  automoc(${devicelistui_STAT_SRCS})
  add_library(devicelistui STATIC ${devicelistui_STAT_SRCS})
endif(NOT QT_ONLY)
