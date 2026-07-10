/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_LFM_ON_TRIGGER_H
#define INCLUDED_LFMTOOLS_LFM_ON_TRIGGER_H

#include <gnuradio/lfmTools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lfmTools {

/*!
 * \brief <+description of block+>
 * \ingroup lfmTools
 *
 */
class LFMTOOLS_API LFM_On_Trigger : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<LFM_On_Trigger> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lfmTools::LFM_On_Trigger.
     *
     * To avoid accidental use of raw pointers, lfmTools::LFM_On_Trigger's
     * constructor is in a private implementation
     * class. lfmTools::LFM_On_Trigger::make is the public interface for
     * creating new instances.
     */
    static sptr make(float bandwidth = 1e6,
                     float pulse_width = 500e-6,
                     float samp_rate = 2e6,
                     float amplitude = 0.5);
                     
    virtual void set_bandwidth(float bandwidth) = 0;
    virtual void set_pulse_width(float pulse_width) = 0;
    virtual void set_samp_rate(float samp_rate) = 0;
    virtual void set_amplitude(float amplitude) = 0;
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_LFM_ON_TRIGGER_H */
