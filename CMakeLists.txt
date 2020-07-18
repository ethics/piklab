project(piklab)
set(VERSION "0.16.2")


if(WIN32)
  set(HAVE_USB 1)
  if (HAVE_USB)
    set(LIBUSB_VERSION "1.14.3")
    set(LIBUSB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libusb-win32-bin-1.2.6.0)
  endif(HAVE_USB)
endif(WIN32)


cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0011 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(Piklab)


add_subdirectory(src)

if(NOT QT_ONLY)
  add_subdirectory(doc)
endif(NOT QT_ONLY)

if(NOT WIN32)
  add_subdirectory(po)
  add_subdirectory(man)
  add_subdirectory(udev)
  install(FILES README Changelog COPYING DESTINATION share/doc/piklab)
endif(NOT WIN32)
