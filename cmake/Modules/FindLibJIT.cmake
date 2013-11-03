#
# Try to find libjit
#

## From: http://code.google.com/p/intrinsic-noise-analyzer/

# Search for libraries
find_library(LIBJIT_LIBRARY
             NAMES jit
             PATH_SUFFIXES x86 x86_64)

find_library(LIBJIT_PLUS_LIBRARY
             NAMES jitplus
             PATH_SUFFIXES x86 x86_64)

find_path(LIBJIT_INCLUDE_DIR jit/jit.h jit/jit-plus.h
          PATH_SUFFIXES jit)

SET(LIBJIT_LIBRARIES ${LIBJIT_LIBRARY} ${LIBJIT_PLUS_LIBRARY})
SET(LIBJIT_INCLUDE_DIRS ${LIBJIT_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBJIT DEFAULT_MSG
                                  LIBJIT_LIBRARIES LIBJIT_INCLUDE_DIRS)

mark_as_advanced(LIBJIT_LIBRARIES LIBJIT_INCLUDE_DIRS)
