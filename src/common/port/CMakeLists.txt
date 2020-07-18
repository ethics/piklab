include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(port_STAT_SRCS
   parallel.cpp
   port.cpp
   serial.cpp
   usb_port.cpp
   port_base.cpp
   hid_port.cpp
)

automoc(${port_STAT_SRCS})
add_library(port STATIC ${port_STAT_SRCS})
