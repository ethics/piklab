include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(nokde_gui_STAT_SRCS
    nokde_misc_ui.cpp
    nokde_dialog.cpp
    nokde_klistview.cpp
    nokde_text_editor.cpp
    nokde_dockwindow.cpp
)

automoc(${nokde_gui_STAT_SRCS})

add_library(nokde_gui STATIC ${nokde_gui_STAT_SRCS})
