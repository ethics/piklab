include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/tbl_bootloader_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_tbl_bootloader_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_tbl_bootloader_parser tbl_bootloader.xml
)

SET(tblbootloader_STAT_SRCS
   tbl_bootloader_data.cpp
   tbl_bootloader.cpp
   tbl_bootloader_prog.cpp
)
automoc(${tblbootloader_STAT_SRCS})
add_library(tblbootloader STATIC ${tblbootloader_STAT_SRCS})
