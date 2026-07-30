/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "peak_cancel_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lfmTools {

using input_type = float;
using output_type = float;
peak_cancel::sptr peak_cancel::make(float bandwidth, float pulse_width, float samp_rate, std::string key) {
    return gnuradio::make_block_sptr<peak_cancel_impl>(bandwidth, pulse_width, samp_rate, key);
}


/*
 * The private constructor
 */
peak_cancel_impl::peak_cancel_impl(float bandwidth, float pulse_width, float samp_rate, std::string key)
    : gr::sync_block("peak_cancel",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))),
    d_bandwidth(bandwidth),
    d_pulse_width(pulse_width), 
    d_samp_rate(samp_rate),
    d_key(key) {
    generate_peak();
}

/*
 * Our virtual destructor.
 */
peak_cancel_impl::~peak_cancel_impl() {}

void peak_cancel_impl::generate_peak() {
    const float PI = 3.14159265358979323846f;

    int max_sample = static_cast<int>(d_pulse_width * d_samp_rate);
    d_peak.resize(2 * max_sample + 1);

    auto sinc = [PI](float x) -> float {
        if (std::abs(x) < 1e-6f)
            return 1.0f;
        return std::sin(PI * x) / (PI * x);
    };

    for (int n = -max_sample; n <= max_sample; n++)
    {
        float tau = static_cast<float>(n) / d_samp_rate;
        float abs_tau = std::abs(tau);
        float triangle = 1.0f - (abs_tau / d_pulse_width);
        if (triangle < 0.0f)
            triangle = 0.0f;
            
        float sinc_arg = d_bandwidth * triangle * tau;
        float magnitude = triangle * sinc(sinc_arg);
        d_peak[n + max_sample] = std::abs(magnitude);
    }
}

void peak_cancel_impl::set_bandwidth(float bandwidth) {
    d_bandwidth = bandwidth;
    generate_peak();
}

void peak_cancel_impl::set_pulse_width(float pulse_width) {
    d_pulse_width = pulse_width;
    generate_peak();
}

void peak_cancel_impl::set_samp_rate(float samp_rate) {
    d_samp_rate = samp_rate;
    generate_peak();
}

void peak_cancel_impl::set_key(std::string key) {
    d_key = key;
}

int peak_cancel_impl::work(int noutput_items,
                           gr_vector_const_void_star& input_items,
                           gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    std::vector<tag_t> tags;

    uint64_t start = nitems_read(0);
    uint64_t end   = start + noutput_items;
    get_tags_in_range(tags, 0, start, end);

    std::vector<float> peaks(noutput_items, 0.0f);
    for (const auto& tag : tags) {
        size_t index = tag.offset - start;
        if (index < peaks.size()) {
            if (pmt::symbol_to_string(tag.key) == d_key) {
                peaks[index] = pmt::to_float(tag.value);
            }
        }
    }
    
    for (int i = 0; i < noutput_items; i++) {
        if (peaks[i] > 0.0f) {
            d_cancel.resize(d_peak.size(), 0.0f);
            for (size_t j = 0; j < d_peak.size(); j++) {
                d_cancel[j] += peaks[i] * d_peak[j];
            }
        }
        if (d_cancel.size() > 0) {
            float cancel = d_cancel.front();
            d_cancel.pop_front();
            out[i] = in[i] - cancel;
        } else {
            out[i] = in[i];
        }
    }
    
    return noutput_items;
}

} /* namespace lfmTools */
} /* namespace gr */
