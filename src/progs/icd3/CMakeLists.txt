add_subdirectory(icd3_data)
add_subdirectory(xml)
add_subdirectory(base)

if(NOT QT_ONLY)
  add_subdirectory(gui)
endif(NOT QT_ONLY)
