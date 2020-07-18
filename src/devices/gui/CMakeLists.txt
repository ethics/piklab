include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(deviceui_STAT_SRCS
   hex_word_editor.cpp
   memory_editor.cpp
   register_view.cpp
   hex_view.cpp
   device_group_ui.cpp
)

automoc(${deviceui_STAT_SRCS})

add_library(deviceui STATIC ${deviceui_STAT_SRCS})
