/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "LFM_On_Trigger_impl.h"
#include <gnuradio/io_signature.h>
#include <complex.h>
#include <vector>
#include <cstring> 

namespace gr {
namespace lfmTools {

using input_type = float;
using output_type = gr_complex;

constexpr float PI = 3.1415926536f;

LFM_On_Trigger::sptr
LFM_On_Trigger::make(float bandwidth, float pulse_width, float samp_rate, float amplitude)
{
    return gnuradio::make_block_sptr<LFM_On_Trigger_impl>(
        bandwidth, pulse_width, samp_rate, amplitude);
}


/*
 * The private constructor
 */
LFM_On_Trigger_impl::LFM_On_Trigger_impl(float bandwidth, float pulse_width, float samp_rate, float amplitude)
    : gr::sync_block("LFM_On_Trigger",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    _bandwidth = bandwidth;
    _pulse_width = pulse_width;
    _samp_rate = samp_rate;
    _amplitude = amplitude;
    _wrap_around = false;
    _last = 1;
    generate_pulse();
}

void LFM_On_Trigger_impl::set_bandwidth(float bandwidth)
{
    _bandwidth = bandwidth;
    generate_pulse();
}

void LFM_On_Trigger_impl::set_pulse_width(float pulse_width)
{
    _pulse_width = pulse_width;
    generate_pulse();
}

void LFM_On_Trigger_impl::set_samp_rate(float samp_rate)
{
    _samp_rate = samp_rate;
    generate_pulse();
}

void LFM_On_Trigger_impl::set_amplitude(float amplitude)
{
    _amplitude = amplitude;
    generate_pulse();
}

/*
 * Our virtual destructor.
 */
LFM_On_Trigger_impl::~LFM_On_Trigger_impl() {}

void LFM_On_Trigger_impl::generate_pulse() {
    _start = 0;
    int samples_per_pulse = int(_pulse_width * _samp_rate);
    _pulse.resize(samples_per_pulse);

    float ramp_rate = _bandwidth / _pulse_width;
    float time;
    
    gr_complex j(0.0f, 1.0f);
    
    for (int i = 0; i < samples_per_pulse; i++) {
        time = i / _samp_rate;
        _pulse[i] = _amplitude*std::exp(ramp_rate*PI*(time * time)*j);
    }
}

int LFM_On_Trigger_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);
    
    // Set rising size
    int samples_per_pulse = int(_pulse_width * _samp_rate);
    if (_rising.size() < size_t(noutput_items / samples_per_pulse + 2))
        _rising.resize(noutput_items / samples_per_pulse + 2);
    
    // edge case where edge happens at buffer transition
    if (_last < in[0] && !_wrap_around) {
        _start = 0;
        _wrap_around = true;
    }
 
    // new code initialize output to 0
    std::fill(out, out + noutput_items, gr_complex(0.0f, 0.0f));
    
    if (samples_per_pulse - _start > noutput_items && _wrap_around) { // new conditional (added _wrap_around)
        std::memcpy(out, _pulse.data() + _start, noutput_items * sizeof(gr_complex));
        _start += noutput_items;
        _wrap_around = true; // set wrap around to true
        return noutput_items;
        
    } else {
        int skip;
        if (_wrap_around) {
            std::memcpy(out, _pulse.data() + _start, (samples_per_pulse - _start) * sizeof(gr_complex));
            skip = samples_per_pulse - _start;
        } else {
            skip = 0;
        }// New, only copy at start of the the signal if wrap_around is true
        
        // iterate through the list looking for first edge transition
        int first = skip;
        for (; first < noutput_items - 1 && in[first] >= in[first + 1]; first++);
        if (first == noutput_items - 1) { // no transition found, return
            _wrap_around = false;
            return noutput_items;
        } else { // transition found
            // compile list of samples_per_pulse separated rising edges
            int last = 0;
            _rising[0] = first+1;
            for (int i = first + 1; i < noutput_items - 1; i++) {
                if (in[i] < in[i + 1] && i - _rising[last] > samples_per_pulse) {
                    _rising[++last] = i+1;
                }
            }
        
            // populate with pulses
            int edge_i = 0;
            while (edge_i <= last && _rising[edge_i] + samples_per_pulse <= noutput_items) {
                std::memcpy(out + _rising[edge_i], _pulse.data(), samples_per_pulse * sizeof(gr_complex));
                edge_i += 1;
            }
            
            // look at last edge
            if (_rising[last] + samples_per_pulse > noutput_items) {
                int elements_remaining = noutput_items - _rising[last];
                std::memcpy(out + _rising[last], _pulse.data(), elements_remaining * sizeof(gr_complex));
               _start = elements_remaining;
               _wrap_around = true;
            } else {
               _wrap_around = false;
            }
            
            _last = in[noutput_items - 1]; // store last element
            return noutput_items;
        }
    }
    
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lfmTools */
} /* namespace gr */
