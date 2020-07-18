include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(piccui_STAT_SRCS
   picc_ui.cpp
)

automoc(${piccui_STAT_SRCS})

add_library(piccui STATIC ${piccui_STAT_SRCS})
