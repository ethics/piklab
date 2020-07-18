include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(xml_coff_parser_SRCS
   gpprocessor.cpp
   xml_coff_parser.cpp
)
automoc(${xml_coff_parser_SRCS})

add_executable(xml_coff_parser ${xml_coff_parser_SRCS})
target_link_libraries(xml_coff_parser devicelistnoui pic
                      picbase picxml mem24 mem24base mem24xml
                      xmltodata devicebase common
                      ${EXE_LIBS}
)
