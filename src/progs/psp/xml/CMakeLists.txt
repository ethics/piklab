include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(xml_psp_parser_SRCS
   xml_psp_parser.cpp
)

add_executable(xml_psp_parser ${xml_psp_parser_SRCS})
target_link_libraries(xml_psp_parser
                      devicelistnoui pic picbase picxml
                      mem24 mem24base mem24xml xmltodata devicebase common
                      ${EXE_LIBS}
)
