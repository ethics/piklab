include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(boostui_STAT_SRCS
   boost_ui.cpp
)
automoc(${boostui_STAT_SRCS})
add_library(boostui STATIC ${boostui_STAT_SRCS})
