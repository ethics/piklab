include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/icd2_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_icd2_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_icd2_parser icd2.xml
)

SET(icd2_STAT_SRCS
   microchip.cpp
   icd2.cpp
   icd2_prog.cpp
   icd2_serial.cpp
   icd2_usb.cpp
   icd2_usb_firmware.cpp
   icd2_data.cpp
   icd2_debug.cpp
   icd.cpp
   icd_prog.cpp
   icd2_debug_specific.cpp
   icd2_debug_specific_16f.cpp
   icd2_debug_specific_18f.cpp
   icd2_debug_specific_24f.cpp
)
automoc(${icd2_STAT_SRCS})
add_library(icd2 STATIC ${icd2_STAT_SRCS})
