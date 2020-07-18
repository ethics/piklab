include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/icd1_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_icd1_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_icd1_parser icd1.xml
)

SET(icd1_STAT_SRCS
   icd1.cpp
   icd1_prog.cpp
   icd1_serial.cpp
   icd1_data.cpp
)
automoc(${icd1_STAT_SRCS})
add_library(icd1 STATIC ${icd1_STAT_SRCS})
