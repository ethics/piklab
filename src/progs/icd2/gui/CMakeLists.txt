include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(icd2ui_STAT_SRCS
   icd2_group_ui.cpp
)
automoc(${icd2ui_STAT_SRCS})
add_library(icd2ui STATIC ${icd2ui_STAT_SRCS})
