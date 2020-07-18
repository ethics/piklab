include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pic_prog_family_xml_to_data_SRCS
  pic_prog_family_xml_to_data.cpp
)
automoc(${pic_prog_family_xml_to_data_SRCS})
add_executable(pic_prog_family_xml_to_data ${pic_prog_family_xml_to_data_SRCS})
target_link_libraries(pic_prog_family_xml_to_data xmltodata devicebase common ${EXE_LIBS})

FILE(GLOB PROG_XML_DEPEND RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.xml")
ADD_CUSTOM_COMMAND(
  OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/pic_prog_family_data.cpp
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/pic_prog_family_xml_to_data ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pic_prog_family_xml_to_data ${PROG_XML_DEPEND}
)

SET(picbase_STAT_SRCS
   pic.cpp
   pic_config.cpp
   pic_protection.cpp
   pic_register.cpp
   pic_prog_family.cpp
   pic_prog_family_data.cpp
)
automoc(${picbase_STAT_SRCS})
add_library(picbase STATIC ${picbase_STAT_SRCS})
