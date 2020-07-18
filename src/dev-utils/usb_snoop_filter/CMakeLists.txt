include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(usb_snoop_filter_SRCS
    usb_snoop_filter.cpp
)
automoc(${usb_snoop_filter_SRCS})

add_executable(usb_snoop_filter ${usb_snoop_filter_SRCS})
target_link_libraries(usb_snoop_filter
                      cli global common
                      ${EXE_LIBS}
)
