include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(gputilsui_STAT_SRCS
   gputils_ui.cpp
)

automoc(${gputilsui_STAT_SRCS})

add_library(gputilsui STATIC ${gputilsui_STAT_SRCS})
