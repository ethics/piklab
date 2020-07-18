include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(devicebase_STAT_SRCS
   generic_device.cpp
   hex_buffer.cpp
   generic_memory.cpp
   register.cpp
   device_group.cpp
)

automoc(${devicebase_STAT_SRCS})

add_library(devicebase STATIC ${devicebase_STAT_SRCS})
