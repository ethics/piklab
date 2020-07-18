add_subdirectory(app_data)

SET(syntax_xml_generator_SRCS
   syntax_xml_generator.cpp
)
automoc(${syntax_xml_generator_SRCS})
add_executable(syntax_xml_generator ${syntax_xml_generator_SRCS})
target_link_libraries(syntax_xml_generator ${EXE_LIBS})

ADD_CUSTOM_COMMAND(
   OUTPUT  asm-pic.xml coff-pic.xml
   COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && ${CMAKE_CURRENT_BINARY_DIR}/syntax_xml_generator
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/syntax_xml_generator
)
ADD_CUSTOM_TARGET(generate_coff-pic ALL DEPENDS coff-pic.xml)
install(FILES asm-pic.xml coff-pic.xml jal-pic.xml coff-c-pic.xml DESTINATION ${DATA_INSTALL_DIR}/katepart/syntax)
