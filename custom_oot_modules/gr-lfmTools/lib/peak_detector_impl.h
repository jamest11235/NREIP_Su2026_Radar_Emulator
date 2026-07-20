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
    int d_look_ahead;
    float d_thres;
    bool d_found;
    void reset();
     
public:
    peak_detector_impl(float threshold,
                      int look_ahead);
    ~peak_detector_impl() override;

    void set_threshold(float thres) override;
    void set_look_ahead(int look) override;
    float threshold() override {return d_thres; }
    int look_ahead() override { return d_look_ahead; }

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};
}
}


#endif /* INCLUDED_LFMTOOLS_PEAK_DETECTOR_IMPL_H */
