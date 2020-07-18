include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(c18ui_STAT_SRCS
   c18_ui.cpp
)

automoc(${c18ui_STAT_SRCS})

add_library(c18ui STATIC ${c18ui_STAT_SRCS})
