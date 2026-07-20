/* -*- c++ -*- */
/*
 * Copyright 2026 James H Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "peak_detector_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <limits>

namespace gr {
namespace lfmTools {

using input_type = float;
using output_type = char;
peak_detector::sptr peak_detector::make(float threshold,
                                        int look_ahead) {
    return gnuradio::make_block_sptr<peak_detector_impl>(threshold,
                                                         look_ahead);
}

/*
 * The private constructor
 */
peak_detector_impl::peak_detector_impl(float threshold,
                                          int look_ahead)
    : sync_block("peak_detector",
                 io_signature::make2(1, 1, sizeof(input_type), sizeof(char)),
                 io_signature::make(1, 1, sizeof(output_type))),
      d_look_ahead(look_ahead),
      d_thres(threshold),
      d_found(false) {}

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
peak_detector_impl::~peak_detector_impl() {}

int peak_detector_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items)
{
    const input_type* iptr = (const input_type*)input_items[0];
    char* optr = (output_type*)output_items[0];
    
    memset(optr, 0, noutput_items * sizeof(char));

    if (d_found == false) { // have not crossed threshold yet
        for (int i = 0; i < noutput_items; i++) {
            if (iptr[i] >  d_thres) {
                d_found = true;
                set_output_multiple(d_look_ahead);
                return i;
            }
        }
        return noutput_items;
        
    } else if (noutput_items >= d_look_ahead) { // can complete in this call
        float peak_val = iptr[0];
        int peak_ind = 0;
        for (int i = 1; i < d_look_ahead; i++) {
            if (iptr[i] > peak_val) {
                peak_val = iptr[i];
                peak_ind = i;
            }
        }
        optr[peak_ind] = 1;
        d_found = false;
        set_output_multiple(1);
        return d_look_ahead;
    } else { // ask for more
        return 0;
    }
}

} /* namespace lfmTools */
} /* namespace gr */
