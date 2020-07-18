include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(common_STAT_SRCS
   args.cpp
   bitvalue.cpp
   group.cpp
   misc.cpp
   number.cpp
   purl_base.cpp
   storage.cpp
   synchronous.cpp
   version_data.cpp
)
automoc(${common_STAT_SRCS})
add_library(common STATIC ${common_STAT_SRCS})
