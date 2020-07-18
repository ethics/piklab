include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/op_data.cpp
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_op_parser ${CMAKE_CURRENT_SOURCE_DIR}
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../xml/xml_op_parser op.xml
)

SET(op_STAT_SRCS
   op_prog.cpp
   op.cpp
   op_mem24.cpp
   op_data.cpp
   op_pic.cpp
   op_16.cpp
   op_16F.cpp
)
automoc(${op_STAT_SRCS})
add_library(op STATIC ${op_STAT_SRCS})
