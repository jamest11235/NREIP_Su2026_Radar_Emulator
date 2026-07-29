#ifndef INCLUDED_LFMTOOLS_TAG_DELAY_H
#define INCLUDED_LFMTOOLS_TAG_DELAY_H

#include <gnuradio/lfmTools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lfmTools {

class LFMTOOLS_API tag_delay : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<tag_delay> sptr;

    // Factory constructor defaulting delay to 0
    static sptr make(int delay_samples = 0);

    // Dynamic runtime setter and getter
    virtual void set_delay_samples(int delay_samples) = 0;
    virtual int delay_samples() const = 0;
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_TAG_DELAY_H */
