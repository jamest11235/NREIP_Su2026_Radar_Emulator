/* -*- c++ -*- */
/*
 * Copyright 2026 James Henry Tang.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LFMTOOLS_PEAK_FILE_SINK_H
#define INCLUDED_LFMTOOLS_PEAK_FILE_SINK_H

#include <gnuradio/lfmTools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lfmTools {

/*!
 * \brief <+description of block+>
 * \ingroup lfmTools
 *
 */
class LFMTOOLS_API peak_file_sink : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<peak_file_sink> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lfmTools::peak_file_sink.
     *
     * To avoid accidental use of raw pointers, lfmTools::peak_file_sink's
     * constructor is in a private implementation
     * class. lfmTools::peak_file_sink::make is the public interface for
     * creating new instances.
     */
    static sptr make(std::string file = "delay.txt");
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_PEAK_FILE_SINK_H */
