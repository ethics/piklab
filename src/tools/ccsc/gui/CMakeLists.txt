include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(ccscui_STAT_SRCS
   ccsc_ui.cpp
)

automoc(${ccscui_STAT_SRCS})

add_library(ccscui STATIC ${ccscui_STAT_SRCS})
