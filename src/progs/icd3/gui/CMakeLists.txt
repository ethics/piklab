include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(icd3ui_STAT_SRCS
   icd3_group_ui.cpp
)
automoc(${icd3ui_STAT_SRCS})
add_library(icd3ui STATIC ${icd3ui_STAT_SRCS})
