find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_LFMTOOLS gnuradio-lfmTools)

FIND_PATH(
    GR_LFMTOOLS_INCLUDE_DIRS
    NAMES gnuradio/lfmTools/api.h
    HINTS $ENV{LFMTOOLS_DIR}/include
        ${PC_LFMTOOLS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_LFMTOOLS_LIBRARIES
    NAMES gnuradio-lfmTools
    HINTS $ENV{LFMTOOLS_DIR}/lib
        ${PC_LFMTOOLS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-lfmToolsTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_LFMTOOLS DEFAULT_MSG GR_LFMTOOLS_LIBRARIES GR_LFMTOOLS_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_LFMTOOLS_LIBRARIES GR_LFMTOOLS_INCLUDE_DIRS)
