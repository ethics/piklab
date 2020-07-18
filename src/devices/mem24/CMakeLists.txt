add_subdirectory(base)
add_subdirectory(xml)
add_subdirectory(xml_data)
add_subdirectory(mem24)
add_subdirectory(prog)

if(NOT QT_ONLY)
  add_subdirectory(gui)
endif(NOT QT_ONLY)
