include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(xml_pickit2_parser_SRCS
   xml_pickit2_parser.cpp
)

add_executable(xml_pickit2_parser ${xml_pickit2_parser_SRCS})
target_link_libraries(xml_pickit2_parser
                      devicelistnoui pic picbase picxml
                      mem24 mem24base mem24xml xmltodata devicebase common
                      ${EXE_LIBS}
)
