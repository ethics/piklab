include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(xml_direct_parser_SRCS
   xml_direct_parser.cpp
)

add_executable(xml_direct_parser ${xml_direct_parser_SRCS})
target_link_libraries(xml_direct_parser
                      devicelistnoui pic picbase picxml
                      mem24 mem24base mem24xml xmltodata devicebase common
                      ${EXE_LIBS}
)
