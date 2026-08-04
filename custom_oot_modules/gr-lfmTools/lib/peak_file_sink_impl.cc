/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "peak_file_sink_impl.h"
#include <gnuradio/io_signature.h>
#include <fstream>
#include <string>
#include <vector>

namespace gr {
namespace lfmTools {

using input_type = float;
peak_file_sink::sptr peak_file_sink::make(std::string file,
                                          std::string tx_peak_label,
                                          std::string rx_peak_label) {
    return gnuradio::make_block_sptr<peak_file_sink_impl>(file,
                                                          tx_peak_label, 
                                                          rx_peak_label);
}


/*
 * The private constructor
 */
peak_file_sink_impl::peak_file_sink_impl(std::string file,
                                         std::string tx_peak_label,
                                         std::string rx_peak_label)
    : gr::sync_block("peak_file_sink",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(0, 0, 0)),
                     counter(0),
                     d_filename(file),
                     d_tx_peak_label(tx_peak_label),
                     d_rx_peak_label(rx_peak_label),
                     d_port(pmt::mp("delay")),
                     d_tx_last_seen(false),
                     d_wait(false) {
    message_port_register_out(d_port);
}

/*
 * Our virtual destructor.
 */
peak_file_sink_impl::~peak_file_sink_impl() {}

int peak_file_sink_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    std::vector<tag_t> tags;
    uint64_t start = nitems_read(0);
    uint64_t end   = start + noutput_items;
    get_tags_in_range(tags, 0, start, end);
    std::vector<std::string> peaks(noutput_items, "");
    std::vector<bool> reset(noutput_items, false);
    for (const auto& tag : tags) {
        size_t index = tag.offset - start;
        if (index < peaks.size() && pmt::is_symbol(tag.key)) {
            std::string tag_key = pmt::symbol_to_string(tag.key);
            if (tag_key == "reset") {
                reset[index] = true;
            } else {
                peaks[index] = tag_key;
            }
        }
    }
    
    for (int i = 0; i < noutput_items; i++) {
        if (d_wait) {
            if (reset[i]) {
                d_wait = false;
                d_tx_last_seen = false;
                counter = 0;
            }
        } else {
            if (peaks[i] == d_rx_peak_label && d_tx_last_seen) {
                pmt::pmt_t key = pmt::string_to_symbol("delay");
                pmt::pmt_t val = pmt::from_long(counter);
                pmt::pmt_t msg = pmt::cons(key, val);
                message_port_pub(d_port, msg);
                d_wait = true;
                d_tx_last_seen = false;
            } else if (peaks[i] == d_tx_peak_label) {
                counter = 0;
                d_tx_last_seen = true;
            }
        }
        counter++;
    }
    return noutput_items;
}

bool peak_file_sink_impl::stop() {
    // Perform cleanup actions here
    std::ofstream file(d_filename);
    // Check if the file opened successfully
    if (file.is_open()) {
        for (int peak : d_peaks) {
            file << peak << std::endl;
        }
    }
    
    return true;
}

} /* namespace lfmTools */
} /* namespace gr */
