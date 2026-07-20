/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "LFM_Source_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <complex.h>
#include <vector>
#include <cstring>
#include <random>

namespace gr {
namespace lfmTools {

using input_type = float;
using output_type_0 = gr_complex;
using output_type_1 = char;

constexpr float PI = 3.1415926536f;

LFM_Source::sptr
LFM_Source::make(float bandwidth, float duty_cycle, float pri, float samp_rate, float amplitude) {
    return gnuradio::make_block_sptr<LFM_Source_impl>(
        bandwidth, duty_cycle, pri, samp_rate, amplitude);
}

LFM_Source_impl::LFM_Source_impl(float bandwidth, float duty_cycle, float pri, float samp_rate, float amplitude)
    : sync_block("LFM_Source",
                     io_signature::make(
                         0 /* min inputs */, 0 /* max inputs */, 0),
                     io_signature::make2(
                         1 /* min outputs */, 2 /* max outputs */, sizeof(gr_complex), sizeof(char))),
      d_bandwidth(bandwidth),
      d_duty_cycle(duty_cycle),
      d_pri(pri),
      d_samp_rate(samp_rate),
      d_amplitude(amplitude),
      d_port(pmt::mp("reset")) {
    message_port_register_out(d_port);
    generate_signal();
}

void LFM_Source_impl::set_bandwidth(float bandwidth) {
    d_bandwidth = bandwidth;
    generate_signal();
    message_port_pub(d_port, pmt::PMT_NIL);
}

void LFM_Source_impl::set_duty_cycle(float duty_cycle) {
    d_duty_cycle = duty_cycle;
    generate_signal();
    message_port_pub(d_port, pmt::PMT_NIL);
}

void LFM_Source_impl::set_pri(float pri) {
    d_pri = pri;
    generate_signal();
    message_port_pub(d_port, pmt::PMT_NIL);
}

void LFM_Source_impl::set_samp_rate(float samp_rate) {
    d_samp_rate = samp_rate;
    generate_signal();
    message_port_pub(d_port, pmt::PMT_NIL);
}

void LFM_Source_impl::set_amplitude(float amplitude) {
    d_amplitude = amplitude;
    generate_signal();
    message_port_pub(d_port, pmt::PMT_NIL);
}

LFM_Source_impl::~LFM_Source_impl() {}

void LFM_Source_impl::generate_signal() {
    int samples_per_pri = int(d_pri * d_samp_rate);
    d_signal.resize(samples_per_pri);

    set_output_multiple(samples_per_pri);
    
    float pulse_width = d_pri * d_duty_cycle; 
    int samples_per_pulse = int(pulse_width * d_samp_rate);
    float ramp_rate = d_bandwidth / pulse_width;
    float time;
    
    gr_complex j(0.0f, 1.0f);
    
    for (int i = 0; i < samples_per_pri; i++) {
        if (i < samples_per_pulse) {
            time = i / d_samp_rate;
            d_signal[i] = d_amplitude*std::exp(ramp_rate*PI*(time * time)*j);
        } else {
            d_signal[i] = 0;
        }
    }
}

int LFM_Source_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items) {
    //auto in = static_cast<const input_type*>(input_items[0]);
    int samples_per_pri = int(d_pri * d_samp_rate);
    auto out = (gr_complex*)(output_items[0]);
    for (int i = 0; i < noutput_items; i += samples_per_pri)
        std::memcpy(out + i, d_signal.data(), samples_per_pri * sizeof(gr_complex));
        
    return noutput_items;
}

} /* namespace lfmTools */
} /* namespace gr */
