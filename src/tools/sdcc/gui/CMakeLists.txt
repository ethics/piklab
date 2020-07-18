include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(sdccui_STAT_SRCS
   sdcc_ui.cpp
)

automoc(${sdccui_STAT_SRCS})

add_library(sdccui STATIC ${sdccui_STAT_SRCS})
