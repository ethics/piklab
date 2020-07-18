include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(save_load_memory_check_SRCS
   save_load_memory_check.cpp
)
automoc(${save_load_memory_check_SRCS})
add_executable(save_load_memory_check ${save_load_memory_check_SRCS})
target_link_libraries(save_load_memory_check
                                 test cli global coff devicelistnoui
                                 pic picbase picxml mem24 mem24base mem24xml
                                 devicebase common
                                 ${EXE_LIBS}
)
