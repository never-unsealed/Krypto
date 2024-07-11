# - Find gengetopt
# This module looks for gengetopt. This module defines the
# following values:
#  GENGETOPT_EXECUTABLE: the full path to the gengetopt tool.
#  GENGETOPT_FOUND: True if gengetopt has been found.

SET(GENGETOPT_FOUND "YES")

FIND_PROGRAM(GENGETOPT_EXECUTABLE
  gengetopt
  /usr/bin /usr/local/bin
)

IF (NOT GENGETOPT_EXECUTABLE)
  SET(GENGETOPT_FOUND "NO")
  IF (GENGETOPT_FIND_REQUIRED) 
    MESSAGE(FATAL_ERROR  "Could not find gengetopt executable")
  ENDIF (GENGETOPT_FIND_REQUIRED) 
ENDIF (NOT GENGETOPT_EXECUTABLE)

IF (GENGETOPT_FOUND)
  IF (NOT GENGETOPT_FIND_QUIETLY)
    MESSAGE(STATUS "Found gengetopt in...: ${GENGETOPT_EXECUTABLE}")
  ENDIF (NOT GENGETOPT_FIND_QUIETLY)
ELSE (GENGETOPT_FOUND)
  IF (NOT GENGETOPT_FIND_QUIETLY)
    MESSAGE(FATAL_ERROR  "Could not find gengetopt binary")
  ENDIF (NOT GENGETOPT_FIND_QUIETLY)
ENDIF (GENGETOPT_FOUND)

mark_as_advanced( GENGETOPT_EXECUTABLE )

