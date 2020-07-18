include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/icd3_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_icd3_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_icd3_parser icd3.xml
)

SET(icd3_STAT_SRCS
   icd3.cpp
   icd3_prog.cpp
   icd3_usb.cpp
   icd3_data.cpp
)
automoc(${icd3_STAT_SRCS})
add_library(icd3 STATIC ${icd3_STAT_SRCS})
