include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(piklab_prog_SRCS
    cmdline.cpp
    cli_prog_manager.cpp
    cli_debug_manager.cpp
)
automoc(${piklab_prog_SRCS})

add_executable(piklab-prog ${piklab_prog_SRCS})
target_link_libraries(piklab-prog
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

install(TARGETS piklab-prog DESTINATION bin)
