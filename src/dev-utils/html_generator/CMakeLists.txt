include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(html_generator_SRCS
    html_generator.cpp
)
automoc(${html_generator_SRCS})
add_executable(html_generator ${html_generator_SRCS})
target_link_libraries(html_generator
                      generator proglistnoui
                      picdembootloader pickit2bootloader tblbootloader
                      bootloader gpsim psp pickit1
                      pickit3 pickit3data pickit2v2 pickit2 icd3 icd1 icd2 icd2data directprog
                      mem24prog picprog progmanager progbase
                      coff port cli devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase global common
                      ${EXE_LIBS}
                      ${LIBUSB_LIBRARIES}
)
