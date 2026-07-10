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
    float _bandwidth;
    float _pulse_width;
    float _samp_rate;
    float _amplitude;
    std::vector<gr_complex> _pulse;
    int _start;
    bool _wrap_around; // new wraparound state holder
    
    float _last;
    // edge case where the rising edge happens from one buffer to another, e.g. prev_buffer_in[-1] = 0 and next_buffer_out[0] = 1

    void generate_pulse();
    
    std::vector<int> _rising;

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
