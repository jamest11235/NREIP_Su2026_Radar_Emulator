# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT lfm_experimental_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT lfm_experimental_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND lfm_experimental_python_files
    lfm_on_trigger_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(lfm_experimental
   ../../..
   gr::lfm_experimental
   "${lfm_experimental_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET lfm_experimental_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lfm_experimental_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/lfm_experimental/
)

install(TARGETS lfm_experimental_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/lfm_experimental COMPONENT pythonapi)
