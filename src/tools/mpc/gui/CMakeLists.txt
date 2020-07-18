include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(mpcui_STAT_SRCS
   mpc_ui.cpp
)

automoc(${mpcui_STAT_SRCS})

add_library(mpcui STATIC ${mpcui_STAT_SRCS})
