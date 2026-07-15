/* -*- c++ -*- */
/*
 * Copyright 2026 James H Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_PEAK_DETECTOR_IMPL_H
#define INCLUDED_LFMTOOLS_PEAK_DETECTOR_IMPL_H

#include <gnuradio/lfmTools/peak_detector.h>

namespace gr {
namespace lfmTools {

class peak_detector_impl : public peak_detector
{
private:
    float d_threshold_factor_rise;
    int d_look_ahead;
    int d_time_since_calib;
    int d_calibration_interval;
    int d_calibration_window;
    float d_thres;
    bool d_calibration_state;
    bool d_found;

public:
    peak_detector_impl(float threshold_factor_rise,
                      int look_ahead,
                      int calibration_window,
                      int calibration_interval);
    ~peak_detector_impl() override;

    void set_threshold_factor_rise(float thr) override;
    void set_look_ahead(int look) override;
    void set_calibration_window(float calib_window) override { d_calibration_window = calib_window;}
    void set_calibration_state(float calib_state) override { d_calibration_state = calib_state; }
    float threshold_factor_rise() override { return d_threshold_factor_rise; }
    int look_ahead() override { return d_look_ahead; }

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
             
};
}
}


#endif /* INCLUDED_LFMTOOLS_PEAK_DETECTOR_IMPL_H */
