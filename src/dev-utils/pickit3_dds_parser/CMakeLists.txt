include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pickit3_dds_parser_SRCS
    pickit3_dds_parser.cpp
)
automoc(${pickit3_dds_parser_SRCS})

add_executable(pickit3_dds_parser ${pickit3_dds_parser_SRCS})
target_link_libraries(pickit3_dds_parser
                      proglistnoui
                      picdembootloader pickit2bootloader tblbootloader
                      bootloader gpsim psp pickit1
                      pickit3 pickit3data pickit2v2 pickit2 icd3 icd1 icd2 icd2data directprog
                      mem24prog picprog progmanager progbase
                      coff port cli global devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase common
                      ${EXE_LIBS}
                      ${LIBUSB_LIBRARIES}
)
