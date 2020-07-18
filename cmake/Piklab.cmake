#set(CMAKE_VERBOSE_MAKEFILE ON)

if(WIN32)
  set(CMAKE_BUILD_TYPE Release)
  set(QT_ONLY 1)
else(WIN32)
  set(CMAKE_BUILD_TYPE Debug)
endif(WIN32)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src)

find_package(Qt4 REQUIRED)
set(QT_USE_QTXML 1)
set(QT_USE_QT3SUPPORT 1)
set(QT_USE_QTTEST 1)
include(${QT_USE_FILE})

macro(automoc)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  qt4_automoc(${ARGV})
endmacro(automoc)

if(WIN32)
  if (HAVE_USB)
    if(NOT EXISTS "${LIBUSB_DIR}")
      message(FATAL_ERROR "Could not find usb path \"${LIBUSB_DIR}\"")
    endif()
    set(LIBUSB_INCLUDE_DIRS ${LIBUSB_DIR}/include)
    if(NOT EXISTS "${LIBUSB_INCLUDE_DIRS}/usb.h")
      add_definitions(-DLIBUSB_NEW_WIN32_HEADER)
    endif()
    set(LIBUSB_LIBRARIES ${LIBUSB_DIR}/lib/gcc/libusb.a)
    include_directories(${LIBUSB_INCLUDE_DIRS})
  endif(HAVE_USB)

  ADD_DEFINITIONS(-static-libgcc)
  SET(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -static-libgcc")
else(WIN32)
  find_package(LIBUSB REQUIRED)
  include_directories(${LIBUSB_INCLUDE_DIRS})

  # required for compilation on Slackware
  find_package(NCurses REQUIRED)
  include_directories(${NCURSES_INCLUDE_DIRS})

  find_package(Readline REQUIRED)
  include_directories(${READLINE_INCLUDE_DIRS})

  find_package(ParallelPortSupport)
endif(WIN32)

if(QT_ONLY)
  add_definitions(-DNO_KDE)
  set(EXE_LIBS ${QT_LIBRARIES}  ${READLINE_LIBRARIES} ${NCURSES_LIBRARIES})
  set(GUI_LIBS nokde_gui ${EXE_LIBS} ${LIBUSB_LIBRARIES})
else(QT_ONLY)
  find_package(KDE4 REQUIRED)
  add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
  include_directories( ${KDE4_INCLUDES})
  set(EXE_LIBS ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${READLINE_LIBRARIES} ${NCURSES_LIBRARIES})
  set(GUI_LIBS ${KDE4_KDEUI_LIBS} ${KDE4_KDE3SUPPORT_LIBS} ${EXE_LIBS} kde_gui ${KDE4_KTEXTEDITOR_LIBS} ${KDE4_KFILE_LIBS} ${LIBUSB_LIBRARIES})
endif(QT_ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
