#ifndef INCLUDED_LFMTOOLS_RESET_TAGGER_H
#define INCLUDED_LFMTOOLS_RESET_TAGGER_H

#include <gnuradio/sync_block.h>
#include <gnuradio/lfmTools/api.h>

namespace gr {
namespace lfmTools {

class LFMTOOLS_API reset_tagger : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<reset_tagger> sptr;

    static sptr make(uint64_t wait);
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_RESET_TAGGER_H */
