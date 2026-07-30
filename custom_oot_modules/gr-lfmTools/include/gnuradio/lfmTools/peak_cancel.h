/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_PEAK_CANCEL_H
#define INCLUDED_LFMTOOLS_PEAK_CANCEL_H

#include <gnuradio/lfmTools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lfmTools {

/*!
 * \brief <+description of block+>
 * \ingroup lfmTools
 *
 */
class LFMTOOLS_API peak_cancel : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<peak_cancel> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lfmTools::peak_cancel.
     *
     * To avoid accidental use of raw pointers, lfmTools::peak_cancel's
     * constructor is in a private implementation
     * class. lfmTools::peak_cancel::make is the public interface for
     * creating new instances.
     */
    static sptr
    make(float bandwidth = 300e3, float pulse_width = 100e-6, float samp_rate = 2e6, std::string key = "peak");
    
    virtual void set_bandwidth(float bandwidth) = 0;
    virtual void set_pulse_width(float pulse_width) = 0;
    virtual void set_samp_rate(float samp_rate) = 0;
    virtual void set_key(std::string) = 0;
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_PEAK_CANCEL_H */
