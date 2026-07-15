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
peak_detector::sptr peak_detector::make(float threshold_factor_rise,
                                        int look_ahead,
                                        int calibration_window,
                                        int calibration_interval)
{
    return gnuradio::make_block_sptr<peak_detector_impl>(threshold_factor_rise,
                                                         look_ahead,
                                                         calibration_window,
                                                         calibration_interval);
}


/*
 * The private constructor
 */
peak_detector_impl::peak_detector_impl(float threshold_factor_rise,
                                          int look_ahead,
                                          int calibration_window,
                                          int calibration_interval)
    : sync_block("peak_detector",
                 io_signature::make(1, 1, sizeof(input_type)),
                 io_signature::make(1, 1, sizeof(output_type))),
      d_threshold_factor_rise(threshold_factor_rise),
      d_look_ahead(look_ahead),
      d_time_since_calib(std::numeric_limits<int>::infinity()),
      d_calibration_interval(calibration_interval),
      d_calibration_window(calibration_window),
      d_thres(0),
      d_calibration_state(true),
      d_found(false)
{
    set_output_multiple(d_calibration_window);
}

 void peak_detector_impl::set_threshold_factor_rise(float thr) {
    d_threshold_factor_rise = thr;
    d_found = true;
    set_output_multiple(1);
}

void peak_detector_impl::set_look_ahead(int look) {
    d_look_ahead = look;
    d_found = true;
    set_output_multiple(1);
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
    
    if (d_calibration_state) { // calibrate
        d_thres = 0;
        for (int i; i < d_calibration_window; i++) {
            d_thres = std::max(d_thres, iptr[i]);
        }
        d_time_since_calib = 0;
        d_calibration_state = false;
        set_output_multiple(1);
        return 0;
    } else if (d_time_since_calib >= d_calibration_window) { // set up calibration
        d_calibration_state = true;
        set_output_multiple(d_calibration_window);
        return 0;
    } else {
        memset(optr, 0, noutput_items * sizeof(char));

        gr::thread::scoped_lock lock(d_setlock);

        // have not crossed threshold yet
        if (d_found == false) {
            for (int i = 0; i < noutput_items; i++) {
                if (iptr[i] >  d_thres * d_threshold_factor_rise) {
                    d_found = true;
                    set_output_multiple(d_look_ahead);
                    d_time_since_calib += i;
                    return i;
                }
            }
            d_time_since_calib += noutput_items;
            return noutput_items;
        } // end d_found==false

        // can complete in this call
        else if (noutput_items >= d_look_ahead) {
            float peak_val = iptr[0];
            int peak_ind = 0;
                /*
                    * Loop starts at the second sample because the first one has already been
                    * filtered (see above). Result of the maximum search is correct due
                    * to initialisations above.
                */
            for (int i = 1; i < d_look_ahead; i++) {
                if (iptr[i] > peak_val) {
                    peak_val = iptr[i];
                    peak_ind = i;
                }
            }
            optr[peak_ind] = 1;

            // restart the search
            d_found = false;
            set_output_multiple(1);
            
            d_time_since_calib += d_look_ahead;
            return d_look_ahead;
        } // end can complete in this call

        // cannot complete in this call
        else {
            return 0; // ask for more
        }
    }
}

} /* namespace lfmTools */
} /* namespace gr */
