include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(pickit2_to_data_SRCS
    pickit2_to_data.cpp
)
automoc(${pickit2_to_data_SRCS})
add_executable(pickit2_to_data ${pickit2_to_data_SRCS})
target_link_libraries(pickit2_to_data
                      devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase global common
                      ${EXE_LIBS}
)
