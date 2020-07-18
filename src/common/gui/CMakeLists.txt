include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(commonui_STAT_SRCS
    config_widget.cpp
    container.cpp
    dialog.cpp
    editlistbox.cpp
    hexword_gui.cpp
    misc_gui.cpp
    list_container.cpp
    number_gui.cpp
    list_view.cpp
    purl_gui.cpp
)

automoc(${commonui_STAT_SRCS})

add_library(commonui STATIC ${commonui_STAT_SRCS})
