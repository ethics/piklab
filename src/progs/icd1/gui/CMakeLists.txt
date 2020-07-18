include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(icd1ui_STAT_SRCS
   icd1_group_ui.cpp
)
automoc(${icd1ui_STAT_SRCS})
add_library(icd1ui STATIC ${icd1ui_STAT_SRCS})
