/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "LFM_On_Trigger_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/logger.h>
#include <complex.h>
#include <vector>
#include <cstring> 
#include <cassert>
#include <sstream>

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
    _d_trigger = false;
    _last = 0;
    // this->set_output_multiple(_MAX_PULSE_SAMPLES);
    _bandwidth = bandwidth;
    _pulse_width = pulse_width;
    _samp_rate = samp_rate;
    _amplitude = amplitude;
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
    int samples_per_pulse = int(_pulse_width * _samp_rate);
    _pulse.resize(samples_per_pulse);
    
    if (samples_per_pulse > _MAX_PULSE_SAMPLES) {
        std::stringstream ss;
        ss << "Samples per pulse " << samples_per_pulse
          << " is greater than the maximum allowable samples "
          << _MAX_PULSE_SAMPLES << ".";
        d_logger->error(ss.str());
    }
    assert(samples_per_pulse <= _MAX_PULSE_SAMPLES);

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
    
    if (_d_trigger) {
        int pulse_size = _pulse.size();
        std::memcpy(out, _pulse.data(), pulse_size * sizeof(output_type));
        _d_trigger = false;
        set_output_multiple(1);
        return _pulse.size();
        /*
        if (in[pulse_size - 1] > in[pulse_size - 2]) {
            _d_trigger = true;
        } else {
            _d_trigger = false;
        }
        return pulse_size; */
    } else {
        /*
        if (in[0] > _last) {
            _d_trigger = true;
            return 0;
        }
        */
        for (int i = 0; i < noutput_items; i++) {
            out[i] = 0;
            if (in[i] != 0) {
                _d_trigger = true;
                set_output_multiple(_pulse.size());
                return i;
            }
        }
        // _last = in[noutput_items - 1];
        return noutput_items;
    }
    
    // Tell runtime system how many output items we produced.
}

} /* namespace lfmTools */
} /* namespace gr */
