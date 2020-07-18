include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(piklab_coff_SRCS
   main.cpp
)
automoc(${piklab_coff_SRCS})

add_executable(piklab-coff ${piklab_coff_SRCS})
target_link_libraries(piklab-coff
                      cli coff global
                      devicelistnoui pic picbase picxml mem24 mem24base mem24xml
                      devicebase common
                      ${EXE_LIBS}
)

install(TARGETS piklab-coff DESTINATION bin)
