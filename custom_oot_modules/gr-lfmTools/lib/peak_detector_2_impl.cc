/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "peak_detector_2_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lfmTools {

using input_type = float;
using output_type = char;
peak_detector_2::sptr peak_detector_2::make(float threshold,
                                            float bandwidth,
                                            float pulse_width,
                                            float samp_rate)
{
    return gnuradio::make_block_sptr<peak_detector_2_impl>(
        threshold, bandwidth, pulse_width, samp_rate);
}


/*
 * The private constructor
 */
peak_detector_2_impl::peak_detector_2_impl(float threshold,
                                           float bandwidth,
                                           float pulse_width,
                                           float samp_rate)
    : sync_block("peak_detector",
                 io_signature::make2(1, 1, sizeof(input_type), sizeof(char)),
                 io_signature::make(1, 1, sizeof(output_type))),
      d_threshold(threshold)
      d_bandwidth(bandwidth),
      d_pulse_width(pulse_width),
      d_samp_rate(samp_rate),
      d_found(false),
      d_lookahead(int(pulse_width * samp_rate)) {
    generate_matched();      
}

void peak_detector_impl::reset() {
    d_found = false;
    set_output_multiple(1);
}

void peak_detector_impl::set_threshold(float thres) {
    d_thres = thres;
    reset();
}

void peak_detector_impl::set_look_ahead(int look) {
    d_look_ahead = look;
    reset();
}

/*
 * Our virtual destructor.
 */
peak_detector_2_impl::~peak_detector_2_impl() {}

int peak_detector_2_impl::work(int noutput_items,
                               gr_vector_const_void_star& input_items,
                               gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

#pragma message("Implement the signal processing in your block and remove this warning")
    // Do <+signal processing+>

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lfmTools */
} /* namespace gr */
