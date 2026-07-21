/* -*- c++ -*- */
/*
 * Copyright 2026 James H Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_PEAK_DETECTOR_H
#define INCLUDED_LFMTOOLS_PEAK_DETECTOR_H

#include <gnuradio/lfmTools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lfmTools {

/*!
 * \brief <+description of block+>
 * \ingroup lfmTools
 *
 */
class LFMTOOLS_API peak_detector : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<peak_detector> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lfmTools::peak_detector.
     *
     * To avoid accidental use of raw pointers, lfmTools::peak_detector's
     * constructor is in a private implementation
     * class. lfmTools::peak_detector::make is the public interface for
     * creating new instances.
     */
    static sptr make(float threshold = 0.01,
                     int look_ahead = 100);

    virtual void set_threshold(float thres) = 0;
    virtual void set_look_ahead(int look) = 0;
    virtual float threshold() = 0;
    virtual int look_ahead() = 0;

};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_PEAK_DETECTOR_H */
