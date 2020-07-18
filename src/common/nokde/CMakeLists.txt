include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(nokde_STAT_SRCS
  nokde_kaboutdata.cpp
  nokde_kcmdlineargs.cpp
  nokde_klocale.cpp
  nokde_kurl.cpp
  nokde_kprocess.cpp
)

if(WIN32)
  set(nokde_STAT_SRCS ${nokde_STAT_SRCS}
    win32_utils.c
  )
endif(WIN32)

automoc(${nokde_STAT_SRCS})
add_library(nokde STATIC ${nokde_STAT_SRCS})
