include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/pickit3_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_pickit3_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_pickit3_parser pickit3.xml
)

SET(pickit3_STAT_SRCS
   pickit3_data.cpp
   pickit3.cpp
   pickit3_prog.cpp
)
automoc(${pickit3_STAT_SRCS})
add_library(pickit3 STATIC ${pickit3_STAT_SRCS})
