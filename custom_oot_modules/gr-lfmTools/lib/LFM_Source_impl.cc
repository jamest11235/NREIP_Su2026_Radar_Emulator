/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "LFM_Source_impl.h"
#include <gnuradio/io_signature.h>
#include <complex.h>
#include <vector>
#include <cstring>
#include <random>

namespace gr {
namespace lfmTools {

using input_type = float;
using output_type = gr_complex;

constexpr float PI = 3.1415926536f;

LFM_Source::sptr
LFM_Source::make(float bandwidth, float duty_cycle, float pri, float samp_rate, float amplitude)
{
    return gnuradio::make_block_sptr<LFM_Source_impl>(
        bandwidth, duty_cycle, pri, samp_rate, amplitude);
}


/*
 * The private constructor
 */
LFM_Source_impl::LFM_Source_impl(float bandwidth, float duty_cycle, float pri, float samp_rate, float amplitude)
    : gr::sync_block("LFM_Source",
                     gr::io_signature::make(
                         0 /* min inputs */, 0 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    _bandwidth = bandwidth;
    _duty_cycle = duty_cycle;
    _pri = pri;
    _samp_rate = samp_rate;
    _amplitude = amplitude;
    generate_signal();
}

void LFM_Source_impl::set_bandwidth(float bandwidth)
{
    _bandwidth = bandwidth;
    generate_signal();
}

void LFM_Source_impl::set_duty_cycle(float duty_cycle)
{
    _duty_cycle = duty_cycle;
    generate_signal();
}

void LFM_Source_impl::set_pri(float pri)
{
    _pri = pri;
    generate_signal();
}

void LFM_Source_impl::set_samp_rate(float samp_rate)
{
    _samp_rate = samp_rate;
    generate_signal();
}

void LFM_Source_impl::set_amplitude(float amplitude)
{
    _amplitude = amplitude;
    generate_signal();
}

/*
 * Our virtual destructor.
 */
LFM_Source_impl::~LFM_Source_impl() {}

void LFM_Source_impl::generate_signal() {
    _start = 0;
    int samples_per_pri = int(_pri * _samp_rate);
    _signal.resize(samples_per_pri);

    set_output_multiple(samples_per_pri);
    
    float pulse_width = _pri * _duty_cycle; 
    int samples_per_pulse = int(pulse_width * _samp_rate);
    float ramp_rate = _bandwidth / pulse_width;
    float time;
    
    gr_complex j(0.0f, 1.0f);
    
    for (int i = 0; i < samples_per_pri; i++) {
        if (i < samples_per_pulse) {
            time = i / _samp_rate;
            _signal[i] = _amplitude*std::exp(ramp_rate*PI*(time * time)*j);
        } else {
            _signal[i] = 0;
        }
    }
}

int LFM_Source_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items)
{
    //auto in = static_cast<const input_type*>(input_items[0]);
    int samples_per_pri = int(_pri * _samp_rate);
    auto out = static_cast<output_type*>(output_items[0]);
    for (int i = 0; i < noutput_items; i += samples_per_pri)
        std::memcpy(out + i, _signal.data(), samples_per_pri * sizeof(gr_complex));
        
    return noutput_items;
}

} /* namespace lfmTools */
} /* namespace gr */
