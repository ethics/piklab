include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(xml_pickit1_parser_SRCS
   xml_pickit1_parser.cpp
)

add_executable(xml_pickit1_parser ${xml_pickit1_parser_SRCS})
target_link_libraries(xml_pickit1_parser
                      devicelistnoui pic picbase picxml
                      mem24 mem24base mem24xml xmltodata devicebase common
                      ${EXE_LIBS}
)
