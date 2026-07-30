/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_PEAK_CANCEL_IMPL_H
#define INCLUDED_LFMTOOLS_PEAK_CANCEL_IMPL_H

#include <gnuradio/lfmTools/peak_cancel.h>
#include <vector>

namespace gr {
namespace lfmTools {

class peak_cancel_impl : public peak_cancel
{
private:
    float d_bandwidth;
    float d_pulse_width;
    float d_samp_rate;
    std::string d_key;
    std::vector<float> d_peak;
    std::deque<float> d_cancel;
    
    void generate_peak();

public:
    peak_cancel_impl(float bandwidth, float pulse_width, float samp_rate, std::string key);
    ~peak_cancel_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
             
    void set_bandwidth(float bandwidth) override;
    void set_pulse_width(float pulse_width) override;
    void set_samp_rate(float samp_rate) override;
    void set_key(std::string key) override;
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_PEAK_CANCEL_IMPL_H */
