#ifndef INCLUDED_LFMTOOLS_TAG_ADVANCE_H
#define INCLUDED_LFMTOOLS_TAG_ADVANCE_H

#include <gnuradio/lfmTools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lfmTools {

class LFMTOOLS_API tag_advance : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<tag_advance> sptr;

    // No itemsize parameter needed—strictly float
    static sptr make(int advance_samples);

    virtual void set_advance_samples(int advance_samples) = 0;
    virtual int advance_samples() const = 0;
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_TAG_ADVANCE_H */
