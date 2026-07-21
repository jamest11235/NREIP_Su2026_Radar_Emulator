/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_PEAK_DETECTOR_2_H
#define INCLUDED_LFMTOOLS_PEAK_DETECTOR_2_H

#include <gnuradio/lfmTools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lfmTools {

/*!
 * \brief <+description of block+>
 * \ingroup lfmTools
 *
 */
class LFMTOOLS_API peak_detector_2 : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<peak_detector_2> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lfmTools::peak_detector_2.
     *
     * To avoid accidental use of raw pointers, lfmTools::peak_detector_2's
     * constructor is in a private implementation
     * class. lfmTools::peak_detector_2::make is the public interface for
     * creating new instances.
     */
    static sptr make(float threshold = 0.01,
                     float bandwidth = 1e6,
                     float pulse_width = 500e-6,
                     float samp_rate = 2e6);
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_PEAK_DETECTOR_2_H */
