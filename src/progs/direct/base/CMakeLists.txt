include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/direct_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_direct_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_direct_parser direct.xml
)

SET(directprog_STAT_SRCS
   direct_pic.cpp
   direct_baseline.cpp
   direct_16.cpp
   direct_16F.cpp
   direct_18.cpp
   direct_18F.cpp
   direct_prog.cpp
   direct_prog_config.cpp
   direct_data.cpp
   direct_mem24.cpp
   direct.cpp
)
automoc(${directprog_STAT_SRCS})
add_library(directprog STATIC ${directprog_STAT_SRCS})
