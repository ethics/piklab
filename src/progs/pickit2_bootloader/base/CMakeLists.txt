include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/pickit2_bootloader_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_pickit2_bootloader_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_pickit2_bootloader_parser pickit2_bootloader.xml
)

SET(pickit2bootloader_STAT_SRCS
   pickit2_bootloader_data.cpp
   pickit2_bootloader_prog.cpp
   pickit2_bootloader.cpp
)
automoc(${pickit2bootloader_STAT_SRCS})
add_library(pickit2bootloader STATIC ${pickit2bootloader_STAT_SRCS})
