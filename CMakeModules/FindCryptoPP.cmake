# - Find CryptoPP
# This module looks for Crypto++ library support
# it will define the following values
#  CRYPTOPP_INCLUDE_DIR  = directory containing cryptlib.h
#  CRYPTOPP_LINK_DIR	 = directory containing libcryptopp.so
#  CRYPTOPP_LIBRARY   	 = Crypto++ library
#  CRYPTOPP_FOUND        = true -> all files were found

# Assume that everything is ok
SET(CRYPTOPP_FOUND "YES")

#
# cryptlib.h
#
FIND_PATH(CRYPTOPP_INCLUDE_DIR 
  cryptlib.h 
  PATHS /usr/local/include/* /opt/local/include/* /usr/include/*
)

IF (NOT CRYPTOPP_INCLUDE_DIR)
  SET(CRYPTOPP_FOUND "NO")
  IF (CRYPTOPP_FIND_REQUIRED) 
    MESSAGE(FATAL_ERROR  "Could not find cryptlib.h")
  ENDIF (CRYPTOPP_FIND_REQUIRED) 
ENDIF (NOT CRYPTOPP_INCLUDE_DIR)

#
#  library
#
FIND_LIBRARY(CRYPTOPP_LIBRARY
  NAMES cryptopp
  PATHS /usr/local/lib /opt/local/lib /usr/lib
) 

IF (NOT CRYPTOPP_LIBRARY)
  SET(CRYPTOPP_FOUND "NO")
  IF (CRYPTOPP_FIND_REQUIRED) 
    MESSAGE(FATAL_ERROR  "Could not find cryptopp library")
  ENDIF (CRYPTOPP_FIND_REQUIRED) 
ENDIF (NOT CRYPTOPP_LIBRARY)

GET_FILENAME_COMPONENT(CRYPTOPP_LINK_DIR ${CRYPTOPP_LIBRARY} PATH )

#
# Checking ...
#
IF (CRYPTOPP_FOUND)
  IF (NOT CRYPTOPP_FIND_QUIETLY)
    MESSAGE(STATUS "Found cryptlib.h in..: ${CRYPTOPP_INCLUDE_DIR}")
    MESSAGE(STATUS "Found cryptopp lib...: ${CRYPTOPP_LIBRARY}")
    MESSAGE(STATUS "Library link dir.....: ${CRYPTOPP_LINK_DIR}")
  ENDIF (NOT CRYPTOPP_FIND_QUIETLY)
ELSE (CRYPTOPP_FOUND)
  IF (NOT CRYPTOPP_FIND_QUIETLY)
    MESSAGE(FATAL_ERROR  "Could not find Crypto++ library")
  ENDIF (NOT CRYPTOPP_FIND_QUIETLY)
ENDIF (CRYPTOPP_FOUND)


MARK_AS_ADVANCED(
  CRYPTOPP_INCLUDE_DIR
  CRYPTOPP_LIBRARY
)
