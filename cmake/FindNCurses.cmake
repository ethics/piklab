# - Try to find Readline
# Once done this will define
# NCURSES_FOUND - System has ncurses
# NCURSES_INCLUDE_DIRS - The ncurses include directories
# NCURSES_LIBRARIES - The libraries needed to use ncurses
# NCURSES_DEFINITIONS - Compiler switches required for using ncurses

find_package ( PkgConfig )
pkg_check_modules ( PC_NCURSES QUIET ncurses )
set ( NCURSES_DEFINITIONS ${PC_NCURSES_CFLAGS_OTHER} )

find_path ( NCURSES_INCLUDE_DIR ncurses.h
      HINTS ${PC_NCURSES_INCLUDEDIR} ${PC_NCURSES_INCLUDE_DIRS}
      PATH_SUFFIXES ncurses )

find_library ( NCURSES_LIBRARY NAMES ncurses
      HINTS ${PC_NCURSES_LIBDIR} ${PC_NCURSES_LIBRARY_DIRS} )

set ( NCURSES_LIBRARIES ${NCURSES_LIBRARY} )
set ( NCURSES_INCLUDE_DIRS ${NCURSES_INCLUDE_DIR} )

include ( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set NCURSES_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args ( ncurses DEFAULT_MSG NCURSES_LIBRARY NCURSES_INCLUDE_DIR )
