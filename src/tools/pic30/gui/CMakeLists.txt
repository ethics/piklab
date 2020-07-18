include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pic30ui_STAT_SRCS
   pic30_ui.cpp
)

automoc(${pic30ui_STAT_SRCS})

add_library(pic30ui STATIC ${pic30ui_STAT_SRCS})
