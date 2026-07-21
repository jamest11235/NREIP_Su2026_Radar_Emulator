/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_PEAK_FILE_SINK_IMPL_H
#define INCLUDED_LFMTOOLS_PEAK_FILE_SINK_IMPL_H

#include <gnuradio/lfmTools/peak_file_sink.h>
#include <string>
#include <vector>

namespace gr {
namespace lfmTools {

class peak_file_sink_impl : public peak_file_sink
{
private:
    int counter;
    std::string d_filename;
    std::vector<int> d_peaks;

public:
    peak_file_sink_impl(std::string file);
    ~peak_file_sink_impl();
    
    bool stop() override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_PEAK_FILE_SINK_IMPL_H */
