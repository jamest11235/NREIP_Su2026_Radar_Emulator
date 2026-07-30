#ifndef INCLUDED_LFMTOOLS_RESET_TAGGER_IMPL_H
#define INCLUDED_LFMTOOLS_RESET_TAGGER_IMPL_H

#include <gnuradio/lfmTools/reset_tagger.h>
#include <atomic>

namespace gr {
namespace lfmTools {

class reset_tagger_impl : public reset_tagger
{
private:
    const uint64_t d_wait;
    std::atomic<int> d_pending_resets{0};
    
    bool d_waiting{false};
    uint64_t d_count{0};

public:
    reset_tagger_impl(uint64_t wait);

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_RESET_TAGGER_IMPL_H */
