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
LFM_On_Trigger::make(float bandwidth, float pulse_width, float samp_rate, float amplitude, std::string key) {
    return gnuradio::make_block_sptr<LFM_On_Trigger_impl>(
        bandwidth, pulse_width, samp_rate, amplitude, key);
}


/*
 * The private constructor
 */
LFM_On_Trigger_impl::LFM_On_Trigger_impl(float bandwidth, float pulse_width, float samp_rate, float amplitude, std::string key)
    : gr::sync_block("LFM_On_Trigger",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))),
      d_bandwidth(bandwidth),
      d_pulse_width(pulse_width),
      d_samp_rate(samp_rate),
      d_amplitude(amplitude),
      d_found(false),
      d_key("RUT") {
    generate_pulse();
}

void LFM_On_Trigger_impl::set_bandwidth(float bandwidth) {
    d_bandwidth = bandwidth;
    generate_pulse();
}

void LFM_On_Trigger_impl::set_pulse_width(float pulse_width) {
    d_pulse_width = pulse_width;
    generate_pulse();
}

void LFM_On_Trigger_impl::set_samp_rate(float samp_rate) {
    d_samp_rate = samp_rate;
    generate_pulse();
}

void LFM_On_Trigger_impl::set_amplitude(float amplitude) {
    d_amplitude = amplitude;
    generate_pulse();
}

void LFM_On_Trigger_impl::set_key(std::string key) {
    d_key = key;
}

/*
 * Our virtual destructor.
 */
LFM_On_Trigger_impl::~LFM_On_Trigger_impl() {}

void LFM_On_Trigger_impl::generate_pulse() {    
    int samples_per_pulse = int(d_pulse_width * d_samp_rate);
    d_pulse.resize(samples_per_pulse);

    float ramp_rate = d_bandwidth / d_pulse_width;
    float time;
    
    gr_complex j(0.0f, 1.0f);
    
    for (int i = 0; i < samples_per_pulse; i++) {
        time = i / d_samp_rate;
        d_pulse[i] = d_amplitude*std::exp(ramp_rate*PI*(time * time)*j);
    }
}

int LFM_On_Trigger_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items) {
    auto out = static_cast<output_type*>(output_items[0]);
    std::vector<tag_t> tags;
    uint64_t start = nitems_read(0);
    uint64_t end = start + noutput_items;
    get_tags_in_range(tags, 0, start, end);
    std::vector<bool> peaks(noutput_items, false);
    for (const auto& tag : tags) {
        size_t index = tag.offset - start;
        if (index < peaks.size() && pmt::is_symbol(tag.key) && pmt::symbol_to_string(tag.key) == d_key) {
            peaks[index] = true;
        }
    }
    
    if (d_found) {
        int pulse_size = d_pulse.size();
        if (pulse_size > noutput_items) return 0;
        std::memcpy(out,
            d_pulse.data(),
            pulse_size * sizeof(output_type));
        d_found = false;
        set_output_multiple(1);
        return d_pulse.size();
    } else {
        for (int i = 0; i < noutput_items; i++) {
            out[i] = 0;
            if (peaks[i]) {
                d_found = true;
                set_output_multiple(d_pulse.size());
                return i + 1;
            }
        }
        return noutput_items;
    }
    return 0;
}

} /* namespace lfmTools */
} /* namespace gr */
