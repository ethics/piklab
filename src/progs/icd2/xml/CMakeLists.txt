include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(xml_icd2_parser_SRCS
   xml_icd2_parser.cpp
)

add_executable(xml_icd2_parser ${xml_icd2_parser_SRCS})
target_link_libraries(xml_icd2_parser
                      icd2data devicelistnoui pic picbase picxml
                      mem24 mem24base mem24xml xmltodata devicebase common
                      ${EXE_LIBS}
)
