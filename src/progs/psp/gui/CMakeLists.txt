include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pspui_STAT_SRCS
   psp_group_ui.cpp
)
automoc(${pspui_STAT_SRCS})
add_library(pspui STATIC ${pspui_STAT_SRCS})
