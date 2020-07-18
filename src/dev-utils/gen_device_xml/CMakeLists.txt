include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(gen_device_xml_SRCS
    gen_device_xml.cpp
)
automoc(${gen_device_xml_SRCS})
add_executable(gen_device_xml ${gen_device_xml_SRCS})
target_link_libraries(gen_device_xml
                      coff devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase common
                      ${EXE_LIBS}
                      ${LIBUSB_LIBRARIES}
)
