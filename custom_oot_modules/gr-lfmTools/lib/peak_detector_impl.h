/* -*- c++ -*- */
/*
 * Copyright 2026 James H Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_PEAK_DETECTOR_IMPL_H
#define INCLUDED_LFMTOOLS_PEAK_DETECTOR_IMPL_H

#include <gnuradio/lfmTools/peak_detector.h>
#include <deque>
#include <queue>
#include <utility>
#include <cstdint>

namespace gr {
namespace lfmTools {

using input_type = float;
using output_type = float;

class peak_detector_impl : public peak_detector
{
private:

    class sliding_window_max {
    private:
        uint64_t d_i;
        uint64_t d_offset;
        int d_window_size;
        output_type d_thres;
        std::queue<std::pair<uint64_t, output_type>> d_q;
        std::deque<std::pair<uint64_t, output_type>> d_dq;
    public:
        sliding_window_max(int window, uint64_t offset, output_type thres);
        std::pair<uint64_t, output_type> get_current() const;
        std::pair<uint64_t, output_type> get_oldest() const;
        int get_size() const;
        void add_value(output_type val);
        void clear(int window, uint64_t offset, output_type thres);
    };
    
    float d_thres;
    int d_lookahead;
    std::string d_key;
    sliding_window_max d_bf_max;
    sliding_window_max d_af_max;
    void clear();
    
public:
    peak_detector_impl(float thres,
                      int lookahead,
                      std::string key);
    ~peak_detector_impl() override;
    void set_lookahead(int look) override;
    void set_key(std::string key) override;
    void set_threshold(int thres) override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};
}
}


#endif /* INCLUDED_LFMTOOLS_PEAK_DETECTOR_IMPL_H */
