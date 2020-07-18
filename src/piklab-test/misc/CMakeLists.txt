include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(misc_check_SRCS
   misc_check.cpp
)
automoc(${misc_check_SRCS})
add_executable(misc_check ${misc_check_SRCS})
target_link_libraries(misc_check
                      cli global common
                      ${EXE_LIBS}
)
