find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_LFM_EXPERIMENTAL gnuradio-lfm_experimental)

FIND_PATH(
    GR_LFM_EXPERIMENTAL_INCLUDE_DIRS
    NAMES gnuradio/lfm_experimental/api.h
    HINTS $ENV{LFM_EXPERIMENTAL_DIR}/include
        ${PC_LFM_EXPERIMENTAL_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_LFM_EXPERIMENTAL_LIBRARIES
    NAMES gnuradio-lfm_experimental
    HINTS $ENV{LFM_EXPERIMENTAL_DIR}/lib
        ${PC_LFM_EXPERIMENTAL_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-lfm_experimentalTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_LFM_EXPERIMENTAL DEFAULT_MSG GR_LFM_EXPERIMENTAL_LIBRARIES GR_LFM_EXPERIMENTAL_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_LFM_EXPERIMENTAL_LIBRARIES GR_LFM_EXPERIMENTAL_INCLUDE_DIRS)
