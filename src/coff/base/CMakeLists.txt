include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/coff_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_coff_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_coff_parser coff.xml
)

SET(coff_STAT_SRCS
   coff.cpp
   gpdis.cpp
   gpopcode.cpp
   disassembler.cpp
   coff_data.cpp
   text_coff.cpp
   cdb_parser.cpp
   coff_archive.cpp
   coff_object.cpp
)
automoc(${coff_STAT_SRCS})
add_library(coff STATIC ${coff_STAT_SRCS})
