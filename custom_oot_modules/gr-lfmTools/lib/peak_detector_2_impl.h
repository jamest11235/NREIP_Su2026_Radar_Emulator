/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_PEAK_DETECTOR_2_IMPL_H
#define INCLUDED_LFMTOOLS_PEAK_DETECTOR_2_IMPL_H

#include <gnuradio/lfmTools/peak_detector_2.h>

namespace gr {
namespace lfmTools {

class peak_detector_2_impl : public peak_detector_2
{
private:
    // Nothing to declare in this block.

public:
    peak_detector_2_impl(float threshold,
                         float bandwidth,
                         float pulse_width,
                         float samp_rate);
    ~peak_detector_2_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_PEAK_DETECTOR_2_IMPL_H */
