include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(inc_parser_SRCS
    inc_parser.cpp
)
automoc(${inc_parser_SRCS})

add_executable(inc_parser ${inc_parser_SRCS})
target_link_libraries(inc_parser
                      coff devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase global common
                      ${EXE_LIBS}
)
