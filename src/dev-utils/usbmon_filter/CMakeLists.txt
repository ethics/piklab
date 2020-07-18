include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(usbmon_filter_SRCS
    usbmon_filter.cpp
)
automoc(${usbmon_filter_SRCS})

add_executable(usbmon_filter ${usbmon_filter_SRCS})
target_link_libraries(usbmon_filter
                      cli global common
                      ${EXE_LIBS}
)
