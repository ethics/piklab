include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(mem24ui_STAT_SRCS
   mem24_hex_view.cpp
   mem24_memory_editor.cpp
   mem24_group_ui.cpp
)

automoc(${mem24ui_STAT_SRCS})

add_library(mem24ui STATIC ${mem24ui_STAT_SRCS})
