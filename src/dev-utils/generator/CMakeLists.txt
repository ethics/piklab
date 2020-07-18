include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(generator_SRCS
    main_generator.cpp
)
automoc(${generator_SRCS})
add_library(generator STATIC ${generator_SRCS})
