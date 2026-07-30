#ifndef INCLUDED_YOUR_MODULE_RESET_TAGGER_H
#define INCLUDED_YOUR_MODULE_RESET_TAGGER_H

#include <gnuradio/sync_block.h>
#include <your_module/api.h>

namespace gr {
namespace your_module {

class YOUR_MODULE_API reset_tagger : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<reset_tagger> sptr;

    static sptr make(uint64_t wait);
};

} // namespace your_module
} // namespace gr

#endif /* INCLUDED_YOUR_MODULE_RESET_TAGGER_H */
