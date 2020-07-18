INCLUDE (CheckIncludeFiles)

# Try to find parport header (Linux)
CHECK_INCLUDE_FILES ("linux/ppdev.h" HAVE_PPDEV)

# Try to find ppbus header (FreeBSD)
CHECK_INCLUDE_FILES ("dev/ppbus/ppi.h" HAVE_PPBUS)