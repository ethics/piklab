
if(EXISTS /lib/udev/rules.d/)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/26-microchip.rules DESTINATION /lib/udev/rules.d/)
endif(EXISTS /lib/udev/rules.d/)
