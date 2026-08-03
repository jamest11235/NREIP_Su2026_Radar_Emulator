/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_LFM_ON_TRIGGER_IMPL_H
#define INCLUDED_LFMTOOLS_LFM_ON_TRIGGER_IMPL_H

#include <vector>
#include <gnuradio/lfmTools/LFM_On_Trigger.h>

namespace gr {
namespace lfmTools {

class LFM_On_Trigger_impl : public LFM_On_Trigger
{
private:
    // Nothing to declare in this block.
    float d_bandwidth;
    float d_pulse_width;
    float d_samp_rate;
    float d_amplitude;
    bool d_found;
    std::vector<gr_complex> d_pulse;
    void generate_pulse();


public:
    LFM_On_Trigger_impl(float bandwidth, float pulse_width, float samp_rate, float amplitude);
    ~LFM_On_Trigger_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
             
    void set_bandwidth(float bandwidth);
    void set_pulse_width(float pulse_width);
    void set_samp_rate(float samp_rate);
    void set_amplitude(float amplitude);

};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_LFM_ON_TRIGGER_IMPL_H */
