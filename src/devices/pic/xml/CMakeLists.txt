include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pic_xml_to_data_SRCS
   pic_xml_to_data.cpp
)
automoc(${pic_xml_to_data_SRCS})
add_executable(pic_xml_to_data ${pic_xml_to_data_SRCS})
target_link_libraries(pic_xml_to_data picbase xmltodata devicebase common ${EXE_LIBS})
