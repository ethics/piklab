include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(reg_extractor_SRCS
    reg_extractor.cpp
)
automoc(${reg_extractor_SRCS})
add_executable(reg_extractor ${reg_extractor_SRCS})
target_link_libraries(reg_extractor
                      devparser
                      devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase global common
                      ${EXE_LIBS}
)