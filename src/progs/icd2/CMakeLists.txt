add_subdirectory(icd2_data)
add_subdirectory(xml)
add_subdirectory(base)

if(NOT QT_ONLY)
  add_subdirectory(gui)
endif(NOT QT_ONLY)
