include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(dev_config_check_SRCS
    dev_config_check.cpp
)
automoc(${dev_config_check_SRCS})
add_executable(dev_config_check ${dev_config_check_SRCS})
target_link_libraries(dev_config_check
                      devparser
                      devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase global common
                      ${EXE_LIBS}
)