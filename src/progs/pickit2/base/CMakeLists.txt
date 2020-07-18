include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/pickit2_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_pickit2_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_pickit2_parser pickit2.xml
)

SET(pickit2_STAT_SRCS
   pickit2_data.cpp
   pickit.cpp
   pickit_prog.cpp
   pickit2.cpp
   pickit2_prog.cpp
)
automoc(${pickit2_STAT_SRCS})
add_library(pickit2 STATIC ${pickit2_STAT_SRCS})
