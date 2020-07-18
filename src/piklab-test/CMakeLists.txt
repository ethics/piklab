add_subdirectory(base)
add_subdirectory(misc)
add_subdirectory(save_load_memory)
add_subdirectory(checksum)

if(NOT QT_ONLY)
  add_subdirectory(generators)
  add_subdirectory(gui)
endif(NOT QT_ONLY)