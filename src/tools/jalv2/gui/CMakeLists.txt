include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(jalv2ui_STAT_SRCS
   jalv2_ui.cpp
)

automoc(${jalv2ui_STAT_SRCS})

add_library(jalv2ui STATIC ${jalv2ui_STAT_SRCS})
