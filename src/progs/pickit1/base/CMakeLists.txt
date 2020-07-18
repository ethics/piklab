include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/pickit1_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_pickit1_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_pickit1_parser pickit1.xml
)

SET(pickit1_STAT_SRCS
   pickit1.cpp
   pickit1_prog.cpp
   pickit1_data.cpp
)
automoc(${pickit1_STAT_SRCS})
add_library(pickit1 STATIC ${pickit1_STAT_SRCS})
