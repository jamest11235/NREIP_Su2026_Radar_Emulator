/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFM_EXPERIMENTAL_LFM_ON_TRIGGER_IMPL_H
#define INCLUDED_LFM_EXPERIMENTAL_LFM_ON_TRIGGER_IMPL_H

#include <gnuradio/lfm_experimental/lfm_on_trigger.h>

namespace gr {
namespace lfm_experimental {

class lfm_on_trigger_impl : public lfm_on_trigger
{
private:
    // Nothing to declare in this block.

public:
    lfm_on_trigger_impl();
    ~lfm_on_trigger_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lfm_experimental
} // namespace gr

#endif /* INCLUDED_LFM_EXPERIMENTAL_LFM_ON_TRIGGER_IMPL_H */
