/* -*- c++ -*- */
/*
 * Copyright 2026 James H Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "peak_detector_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>

namespace gr {
namespace lfmTools {

using input_type = float;
using output_type = float;
peak_detector::sptr peak_detector::make(float alpha,
                                        int look_ahead,
                                        std::string key) {
    return gnuradio::make_block_sptr<peak_detector_impl>(alpha,
                                                         look_ahead, key);
}

/*
 * The private constructor
 */
peak_detector_impl::peak_detector_impl(float alpha,
                                       int look_ahead,
                                       std::string key)
    : sync_block("peak_detector",
                 io_signature::make2(1, 1, sizeof(input_type), sizeof(char)),
                 io_signature::make(1, 1, sizeof(output_type))),
      d_look_ahead(look_ahead),
      d_alpha(alpha),
      d_key(key),
      d_thres(0),
      d_found(false),
      d_reset_requested(true),
      d_wait(0) {
    message_port_register_in(pmt::mp("reset"));
    set_msg_handler(pmt::mp("reset"),
        [this](pmt::pmt_t msg) {
            this->reset();
        });
}

void peak_detector_impl::reset() {
    set_output_multiple(1);
    d_found = false;
    d_thres = 0;
    d_wait = 0;
}

 void peak_detector_impl::set_alpha(float alp) {
    d_alpha = alp;
    reset();
}

void peak_detector_impl::set_look_ahead(int look) {
    d_look_ahead = look;
    reset();
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

    if (d_wait < 500000) {
        int consumed = std::min(500000 - d_wait, noutput_items);
        std::memcpy(optr, iptr, consumed * sizeof(output_type));
        d_wait += consumed;
        return consumed;
    }
    
    if (d_found == false) { // have not crossed threshold yet
        for (int i = 0; i < noutput_items; i++) {
            d_thres = std::max(iptr[i], d_thres);
            if (iptr[i] >  d_thres * d_alpha) {
                d_found = true;
                set_output_multiple(d_look_ahead);
                std::memcpy(optr, iptr, i * sizeof(output_type));
                return i;
            }
        }
        std::memcpy(optr, iptr, noutput_items * sizeof(output_type));
        return noutput_items;
        
    } else if (noutput_items >= d_look_ahead) { // can complete in this call
        float peak_val = iptr[0];
        int peak_ind = 0;
        for (int i = 1; i < d_look_ahead; i++) {
            d_thres = std::max(iptr[i], d_thres);
            if (iptr[i] > peak_val) {
                peak_val = iptr[i];
                peak_ind = i;
            }
        }
        pmt::pmt_t key = pmt::string_to_symbol(d_key);
        pmt::pmt_t val = pmt::from_float(peak_val);
        add_item_tag(0, nitems_written(0) + peak_ind, key, val);        
        std::memcpy(optr, iptr, d_look_ahead * sizeof(output_type));
        d_found = false;
        set_output_multiple(1);
        return d_look_ahead;
    } else { // ask for more
        d_logger->info("NOT ENOUGH VALUES\n");
        return 0;
    }
}

} /* namespace lfmTools */
} /* namespace gr */
