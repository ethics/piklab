include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(xml_picdem_bootloader_parser_SRCS
   xml_picdem_bootloader_parser.cpp
)

add_executable(xml_picdem_bootloader_parser ${xml_picdem_bootloader_parser_SRCS})
target_link_libraries(xml_picdem_bootloader_parser
                      devicelistnoui pic picbase picxml
                      mem24 mem24base mem24xml xmltodata devicebase common
                      ${EXE_LIBS}
)
