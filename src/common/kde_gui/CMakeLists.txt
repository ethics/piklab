include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(kde_gui_STAT_SRCS
    kde_purl_gui.cpp
)

automoc(${kde_gui_STAT_SRCS})
add_library(kde_gui STATIC ${kde_gui_STAT_SRCS})
