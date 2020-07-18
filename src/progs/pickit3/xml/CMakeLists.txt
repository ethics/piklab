include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(xml_pickit3_parser_SRCS
   xml_pickit3_parser.cpp
)

add_executable(xml_pickit3_parser ${xml_pickit3_parser_SRCS})
target_link_libraries(xml_pickit3_parser pickit3data
                      coff devicelistnoui pic picbase picxml
                      mem24 mem24base mem24xml xmltodata devicebase common
                      ${EXE_LIBS}
)
