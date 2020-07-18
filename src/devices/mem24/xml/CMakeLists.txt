include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(mem24_xml_to_data_SRCS
   mem24_xml_to_data.cpp
)
automoc(${mem24_xml_to_data_SRCS})

add_executable(mem24_xml_to_data ${mem24_xml_to_data_SRCS})
target_link_libraries(mem24_xml_to_data mem24base xmltodata devicebase common ${EXE_LIBS})
