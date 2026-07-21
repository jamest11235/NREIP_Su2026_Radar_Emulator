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

using input_type = char;
peak_file_sink::sptr peak_file_sink::make(std::string file) {
    return gnuradio::make_block_sptr<peak_file_sink_impl>(file);
}


/*
 * The private constructor
 */
peak_file_sink_impl::peak_file_sink_impl(std::string file)
    : gr::sync_block("peak_file_sink",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(0, 0, 0)),
                     counter(0),
                     d_filename(file) {}

/*
 * Our virtual destructor.
 */
peak_file_sink_impl::~peak_file_sink_impl() {}

int peak_file_sink_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);

    for (int i = 0; i < noutput_items; i++) {
        if (in[i]) {
            d_peaks.push_back(counter);
            counter = 0;
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
