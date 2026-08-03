/* -*- c++ -*- */
/*
 * Copyright 2026 James H Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "peak_detector_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <cstring>
#include <cstdint>

namespace gr {
namespace lfmTools {

using input_type = float;
using output_type = float;

/*
 * Sliding Window Implementation
 */
peak_detector_impl::sliding_window_max::sliding_window_max(int window, uint64_t offset)
    : d_i(offset), d_offset(offset), d_window_size(window) {}
    
std::pair<uint64_t, output_type> peak_detector_impl::sliding_window_max::get_current() const {
    return d_dq.front();
}

std::pair<uint64_t, output_type> peak_detector_impl::sliding_window_max::get_oldest() const {
    return d_q.front();
}

int peak_detector_impl::sliding_window_max::get_size() const {
    return std::min(static_cast<int>(d_i - d_offset), d_window_size);
}

void peak_detector_impl::sliding_window_max::add_value(output_type val) {
    if (static_cast<int>(d_q.size()) >= d_window_size) {
        d_q.pop();
    }
    d_q.push({d_i, val});
    while (!d_dq.empty() && d_dq.front().first <= d_i - d_window_size) {
        d_dq.pop_front();
    }
    while (!d_dq.empty() && val >= d_dq.back().second) {
        d_dq.pop_back();
    }
    d_dq.push_back({d_i, val});
    d_i++;
}

void peak_detector_impl::sliding_window_max::clear(int window, uint64_t offset) {
    d_i = offset;
    d_offset = offset;
    d_window_size = window;
    d_dq.clear();
    d_q = {};
}

/*
 * Pointer
 */
 
peak_detector::sptr peak_detector::make(float thres,
                                        int lookahead,
                                        std::string key) {
    return gnuradio::make_block_sptr<peak_detector_impl>(thres,
                                                         lookahead, key);
}

/*
 * The private constructor
 */
peak_detector_impl::peak_detector_impl(float thres,
                                       int lookahead,
                                       std::string key)
    : sync_block("peak_detector",
                 io_signature::make2(1, 1, sizeof(input_type), sizeof(char)),
                 io_signature::make(1, 1, sizeof(output_type))),
      d_thres(thres),
      d_lookahead(lookahead),
      d_key(key),
      d_bf_max(lookahead, 0),
      d_af_max(lookahead, lookahead) {}

void peak_detector_impl::set_lookahead(int look) {
    d_lookahead = look;
    clear();
}

void peak_detector_impl::set_threshold(int thres) {
    d_thres = thres;
    clear();
}

void peak_detector_impl::clear() {
    d_bf_max.clear(d_lookahead, nitems_written(0));
    d_af_max.clear(d_lookahead, nitems_written(0) + (uint64_t) d_lookahead);
}

void peak_detector_impl::set_key(std::string key) {
    d_key = key;
}

/*
 * Our virtual destructor.
 */
peak_detector_impl::~peak_detector_impl() {}

int peak_detector_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items)
{   
    const input_type* iptr = (const input_type*)input_items[0];
    output_type* optr = (output_type*)output_items[0];
    
    if (d_bf_max.get_size() < d_lookahead) {
        int fill = std::min(d_lookahead - d_bf_max.get_size(), noutput_items);
        for (int i = 0; i < fill; i++) {
            d_bf_max.add_value(iptr[i]);
            optr[i] = 0;
        }
        std::memcpy(optr, iptr, fill * sizeof(output_type));
        return fill;
    } else if (d_af_max.get_size() < d_lookahead) {
        int fill = std::min(d_lookahead - d_af_max.get_size(), noutput_items);
        std::memcpy(optr, iptr, fill * sizeof(output_type));
        for (int i = 0; i < fill; i++) {
            d_af_max.add_value(iptr[i]);
            optr[i] = 0;
        }
        return fill;
    } else {
        std::memcpy(optr, iptr, noutput_items * sizeof(output_type));
        for (int i = 0; i < noutput_items; i++) {
            const auto next = d_af_max.get_oldest();
            d_bf_max.add_value(next.second);
            d_af_max.add_value(iptr[i]);
            output_type max_bf = d_bf_max.get_current().second;
            output_type max_af = d_af_max.get_current().second;
            if (next.second >= max_bf && next.second >= max_af && next.second > d_thres) {
                pmt::pmt_t key = pmt::string_to_symbol(d_key);
                pmt::pmt_t val = pmt::from_float(next.second);
                add_item_tag(0, next.first + d_lookahead, key, val);
            }
            optr[i] = next.second;
        }
        return noutput_items;
    }
}

} /* namespace lfmTools */
} /* namespace gr */
