# - Try to find libusb
# Once done, this will define:
#  LIBUSB_FOUND
#  LIBUSB_INCLUDE_DIRS
#  LIBUSB_LIBRARIES
#  LIBUSB_VERSION

include(LibFindMacros)

libfind_pkg_check_modules(LIBUSB_PKGCONF libusb)

find_path(LIBUSB_INCLUDE_DIR
  NAMES usb.h
  PATHS ${LIBUSB_PKGCONF_INCLUDE_DIRS}
)

find_library(LIBUSB_LIBRARY
  NAMES usb
  PATHS ${LIBUSB_PKGCONF_LIBRARY_DIRS}
)

set(LIBUSB_PROCESS_INCLUDES LIBUSB_INCLUDE_DIR)
set(LIBUSB_PROCESS_LIBS LIBUSB_LIBRARY)
libfind_process(LIBUSB)

if (LIBUSB_FOUND)
  EXEC_PROGRAM(libusb-config ARGS --version RETURN_VALUE _return_VALUE OUTPUT_VARIABLE LIBUSB_VERSION)
  message(STATUS "LIBUSB version: ${LIBUSB_VERSION}")
else (LIBUSB_FOUND)
  message(FATAL_ERROR "Could not find \"libusb-config\"")
endif (LIBUSB_FOUND)

set(HAVE_USB ${LIBUSB_FOUND})
