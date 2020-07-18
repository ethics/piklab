include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(checksum_check_SRCS
   checksum_check.cpp
)
automoc(${checksum_check_SRCS})

add_executable(checksum_check ${checksum_check_SRCS})
target_link_libraries(checksum_check
                      test cli global devicelistnoui
                      pic picbase picxml mem24 mem24base mem24xml
                      devicebase common
                      ${EXE_LIBS}
)
