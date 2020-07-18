include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(picp_check_SRCS
   picdev.cpp
   picp_check.cpp
)
automoc(${picp_check_SRCS})
add_executable(picp_check ${picp_check_SRCS})
target_link_libraries(picp_check
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
