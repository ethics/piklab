include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(piklab_hex_SRCS
   main.cpp
)
automoc(${piklab_hex_SRCS})

add_executable(piklab-hex ${piklab_hex_SRCS})
target_link_libraries(piklab-hex
                      cli global devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase common
                      ${EXE_LIBS}
)

install(TARGETS piklab-hex DESTINATION bin)
