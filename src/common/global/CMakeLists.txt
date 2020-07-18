include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT WIN32)
  ADD_CUSTOM_COMMAND(
    OUTPUT dummy
    COMMAND ${CMAKE_COMMAND} ARGS -E echo "Forcing svn_revision.h generation"
  )
  ADD_CUSTOM_COMMAND(
    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/svn_revision.h
    DEPENDS dummy
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && ${CMAKE_CURRENT_SOURCE_DIR}/svn_revision.sh
  )
  ADD_CUSTOM_TARGET(generate DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/svn_revision.h)
endif(NOT WIN32)

SET(global_STAT_SRCS
   about.cpp
   generic_config.cpp
   log.cpp
   pfile.cpp
   process.cpp
   progress_monitor.cpp
   purl.cpp
   xml_data_file.cpp
   process_kde.cpp
)
automoc(${global_STAT_SRCS})
add_library(global STATIC ${global_STAT_SRCS})

ADD_DEPENDENCIES(global generate)
