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
peak_detector_impl::sliding_window_max::sliding_window_max(int window)
    : d_i(0), d_window_size(window) {}
    
std::pair<uint64_t, output_type> peak_detector_impl::sliding_window_max::get_current() const {
    return d_dq.front();
}

int peak_detector_impl::sliding_window_max::get_size() const {
    return std::min(static_cast<int>(d_i), d_window_size);
}

void peak_detector_impl::sliding_window_max::add_value(output_type val, bool push) {
    while (!d_dq.empty() && d_dq.front().first <= d_i - d_window_size) {
        d_dq.pop_front();
    }
    if (push) {
        while (!d_dq.empty() && val >= d_dq.back().second) {
            d_dq.pop_back();
       }
       d_dq.push_back({d_i, val});
    }
    d_i++;
}

void peak_detector_impl::sliding_window_max::clear(int window) {
    d_i = 0;
    d_window_size = window;
    d_dq.clear();
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
      d_bf_max(lookahead),
      d_af_max(lookahead) {
    set_history(d_lookahead);
    set_tag_propagation_policy(TPP_DONT);
}

void peak_detector_impl::set_lookahead(int look) {
    d_lookahead = look;
    clear();
}

void peak_detector_impl::set_threshold(int thres) {
    d_thres = thres;
    clear();
}

void peak_detector_impl::clear() {
    d_bf_max.clear(d_lookahead);
    d_af_max.clear(d_lookahead);
    set_history(d_lookahead);
}

void peak_detector_impl::set_key(std::string key) {
    d_key = key;
}

void peak_detector_impl::push(sliding_window_max& max, output_type val) {
    max.add_value(val, val >= d_thres);
}

/*
 * Our virtual destructor.
 */
peak_detector_impl::~peak_detector_impl() {}

int peak_detector_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items)
{   

    std::vector<gr::tag_t> tags;
    get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items + history() - 1);

    for (const auto& tag : tags) {
        uint64_t new_offset = tag.offset + d_lookahead;
        add_item_tag(0, new_offset, tag.key, tag.value, tag.srcid);
    }

    const input_type* iptr = (const input_type*)input_items[0];
    output_type* optr = (output_type*)output_items[0];
    for (int i = 0; i < noutput_items; i++) {
        push(d_bf_max, iptr[i]);
        push(d_af_max, iptr[i + d_lookahead]);
        output_type max_bf = d_bf_max.get_current().second;
        output_type max_af = d_af_max.get_current().second;
        if (iptr[i] >= d_thres && iptr[i] >= max_bf && iptr[i] >= max_af) {
            pmt::pmt_t key = pmt::string_to_symbol(d_key);
            pmt::pmt_t val = pmt::from_float(iptr[i]);
            add_item_tag(0, nitems_written(0) + i, key, val);
        }
        optr[i] = iptr[i];
    }
    return noutput_items;
}

} /* namespace lfmTools */
} /* namespace gr */
