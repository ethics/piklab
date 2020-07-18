include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(picui_STAT_SRCS
   pic_config_editor.cpp
   pic_config_word_editor.cpp
   pic_hex_view.cpp
   pic_memory_editor.cpp
   pic_register_view.cpp
   pic_group_ui.cpp
   pic_prog_group_ui.cpp
)
automoc(${picui_STAT_SRCS})
add_library(picui STATIC ${picui_STAT_SRCS})
