include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(gputils_config_generator_check_SRCS
   gputils_config_generator_check.cpp
)
automoc(${gputils_config_generator_check_SRCS})
add_executable(gputils_config_generator_check ${gputils_config_generator_check_SRCS})
target_link_libraries(gputils_config_generator_check
                      test cli global sdcc gputils coff devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase common
                      ${EXE_LIBS}
)

SET(gputils_template_generator_check_SRCS
   gputils_template_generator_check.cpp
)
automoc(${gputils_template_generator_check_SRCS})
add_executable(gputils_template_generator_check ${gputils_template_generator_check_SRCS})
target_link_libraries(gputils_template_generator_check
                      test cli global sdcc gputils coff devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase common
                      ${EXE_LIBS}
)

SET(sdcc_config_generator_check_SRCS
   sdcc_config_generator_check.cpp
)
automoc(${sdcc_config_generator_check_SRCS})
add_executable(sdcc_config_generator_check ${sdcc_config_generator_check_SRCS})
target_link_libraries(sdcc_config_generator_check
                      test cli global sdcc gputils coff devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase common
                      ${EXE_LIBS}
)
