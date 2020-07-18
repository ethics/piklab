include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(xmltodata_STAT_SRCS
   xml_to_data.cpp
   device_xml_to_data.cpp
)

automoc(${xmltodata_STAT_SRCS})

add_library(xmltodata STATIC ${xmltodata_STAT_SRCS})
