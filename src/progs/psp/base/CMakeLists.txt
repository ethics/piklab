include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/psp_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_psp_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_psp_parser psp.xml
)

SET(psp_STAT_SRCS
   psp_prog.cpp
   psp_serial.cpp
   psp_data.cpp
   psp.cpp
)
automoc(${psp_STAT_SRCS})
add_library(psp STATIC ${psp_STAT_SRCS})
