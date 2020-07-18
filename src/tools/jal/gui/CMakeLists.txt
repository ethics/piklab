include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(jalui_STAT_SRCS
   jal_ui.cpp
)

automoc(${jalui_STAT_SRCS})

add_library(jalui STATIC ${jalui_STAT_SRCS})
