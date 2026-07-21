# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT lfmTools_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT lfmTools_sources)

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
list(APPEND lfmTools_python_files
    LFM_On_Trigger_python.cc
    LFM_Source_python.cc
    peak_detector_python.cc
    peak_file_sink_python.cc
    peak_detector_2_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(lfmTools
   ../../..
   gr::lfmTools
   "${lfmTools_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET lfmTools_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lfmTools_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/lfmTools/
)

install(TARGETS lfmTools_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/lfmTools COMPONENT pythonapi)
