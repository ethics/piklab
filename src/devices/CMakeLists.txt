add_subdirectory(base)
add_subdirectory(pic)
add_subdirectory(mem24)
add_subdirectory(list)

if(NOT QT_ONLY)
  add_subdirectory(gui)
endif(NOT QT_ONLY)
