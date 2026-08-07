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
    : d_i(1), d_window_size(window) {
    d_dq.push_back({0, std::numeric_limits<output_type>::max()});    
}
    
output_type peak_detector_impl::sliding_window_max::get_max() const {
    return d_dq.empty() ? -1 : d_dq.front().second;
}

void peak_detector_impl::sliding_window_max::add_value(output_type val, bool push) {
    while (!d_dq.empty() && d_dq.front().first + d_window_size <= d_i) {
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

/*
 * Pointer
 */
 
peak_detector::sptr peak_detector::make(float thres,
                                        int lookahead,
                                        std::string key) {
    return gnuradio::make_block_sptr<peak_detector_impl>(thres,
                                                         lookahead, key);
}

void peak_detector_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    // make sure all inputs have noutput_items available
    unsigned ninputs = ninput_items_required.size();
    for (unsigned i = 0; i < ninputs; i++)
        ninput_items_required[i] = noutput_items + d_lookahead;
}

/*
 * The private constructor
 */
peak_detector_impl::peak_detector_impl(float thres,
                                       int lookahead,
                                       std::string key)
    : block("peak_detector",
                 io_signature::make(1, 1, sizeof(input_type)),
                 io_signature::make(1, 1, sizeof(output_type))),
      d_thres(thres),
      d_lookahead(lookahead),
      d_key(key),
      d_bf_max(lookahead),
      d_af_max(lookahead) {
    if (lookahead <= 1) {
        throw std::runtime_error("Lookahead must be greater than 1");
    }
    set_lookahead(lookahead);
    d_delta = 0;
}

void peak_detector_impl::set_lookahead(int look) {
    if (look != lkahd()) {
        gr::thread::scoped_lock l(d_setlock);
        int old = lkahd();
        set_history(look + 1);
        declare_sample_delay(history() - 1);
        d_delta += lkahd() - old;
        d_lookahead = look;
        clear();
    }
}

void peak_detector_impl::set_threshold(float thres) {
    d_thres = thres;
    clear();
}

void peak_detector_impl::clear() {
    d_bf_max = sliding_window_max(d_lookahead);
    d_af_max = sliding_window_max(d_lookahead); 
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

int peak_detector_impl::general_work(int noutput_items,
                             gr_vector_int& ninput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items) {

    int cons, ret;
    const input_type* iptr = (const input_type*)input_items[0];
    output_type* optr = (output_type*)output_items[0];

    gr::thread::scoped_lock l(d_setglock);
    assert(input_items.size() == output_items.size());


    // No change in delay; just memcpy ins to outs
    if (d_delta == 0) {
        for (int i = 0; i < noutput_items; i++) {
            push(d_bf_max, iptr[i]);
            push(d_af_max, iptr[i + d_lookahead]);
            if (iptr[i] > d_thres && iptr[i] >= d_bf_max.get_max() && iptr[i] >= d_af_max.get_max()) {
                pmt::pmt_t key = pmt::string_to_symbol(d_key);
                pmt::pmt_t val = pmt::from_float(iptr[i]);
                add_item_tag(0, nitems_written(0) + i, key, val);
            }
            optr[i] = iptr[i];
        }
        cons = noutput_items;
        ret = noutput_items;
    }

    // Skip over d_delta items on the input
    else if (d_delta < 0) {
        int n_to_copy, n_adj;
        int delta = -d_delta;
        n_to_copy = std::max(0, noutput_items - delta);
        n_adj = std::min(delta, noutput_items);
        std::memcpy(optr, iptr + delta, n_to_copy * sizeof(output_type));
        cons = noutput_items;
        ret = n_to_copy;
        delta -= n_adj;
        d_delta = -delta;
    }

    // produce but not consume (inserts zeros)
    else { // d_delta > 0
        int n_from_input, n_padding;
        n_from_input = std::max(0, noutput_items - d_delta);
        n_padding = std::min(d_delta, noutput_items);
        std::memset(optr, 0, n_padding * sizeof(output_type));
        std::memcpy(optr, iptr, n_from_input * sizeof(output_type));
        cons = n_from_input;
        ret = noutput_items;
        d_delta -= n_padding;
    }

    consume_each(cons);
    return ret;
}

} /* namespace lfmTools */
} /* namespace gr */
