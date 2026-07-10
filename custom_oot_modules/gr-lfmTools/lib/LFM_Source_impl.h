/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_LFM_SOURCE_IMPL_H
#define INCLUDED_LFMTOOLS_LFM_SOURCE_IMPL_H

#include <gnuradio/lfmTools/LFM_Source.h>

namespace gr {
namespace lfmTools {

class LFM_Source_impl : public LFM_Source
{
private:
    // Nothing to declare in this block.
    float _bandwidth;
    float _duty_cycle;
    float _pri;
    float _samp_rate;
    float _amplitude;
    std::vector<gr_complex> _signal;
    int _start;

    void generate_signal();

public:
    LFM_Source_impl(float bandwidth, float duty_cycle, float pri, float samp_rate, float amplitude);
    ~LFM_Source_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
             
    void set_bandwidth(float bandwidth);
    void set_duty_cycle(float duty_cycle);
    void set_pri(float pri);
    void set_samp_rate(float samp_rate);
    void set_amplitude(float amplitude);

};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_LFM_SOURCE_IMPL_H */
