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
    void forecast(int noutput_items, gr_vector_int& ninput_items_required) override;
    int d_delta;

    class sliding_window_max {
    private:
        uint64_t d_i;
        int d_window_size;
        std::deque<std::pair<uint64_t, output_type>> d_dq;
    public:
        sliding_window_max(int window);
        output_type get_max() const;
        void add_value(output_type val, bool push);
    };
    
    output_type d_thres;
    int d_lookahead;
    std::string d_key;
    sliding_window_max d_bf_max;
    sliding_window_max d_af_max;
    void push(sliding_window_max& max, output_type val);
    void clear();
    
public:
    peak_detector_impl(float thres,
                      int lookahead,
                      std::string key);
    ~peak_detector_impl() override;
    void set_lookahead(int look) override;
    void set_key(std::string key) override;
    void set_threshold(float thres) override;

    int lkahd() const override { return history() - 1; }

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items) override;
};
}
}


#endif /* INCLUDED_LFMTOOLS_PEAK_DETECTOR_IMPL_H */
