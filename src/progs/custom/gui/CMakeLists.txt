include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(customprogui_STAT_SRCS
   custom_group_ui.cpp
)
automoc(${customprogui_STAT_SRCS})
add_library(customprogui STATIC ${customprogui_STAT_SRCS})
