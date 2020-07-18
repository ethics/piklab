include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/picdem_bootloader_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_picdem_bootloader_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_picdem_bootloader_parser picdem_bootloader.xml
)

SET(picdembootloader_STAT_SRCS
   picdem_bootloader_data.cpp
   picdem_bootloader.cpp
   picdem_bootloader_prog.cpp
)
automoc(${picdembootloader_STAT_SRCS})
add_library(picdembootloader STATIC ${picdembootloader_STAT_SRCS})
