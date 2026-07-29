#ifndef INCLUDED_LFMTOOLS_TAG_ADVANCE_IMPL_H
#define INCLUDED_LFMTOOLS_TAG_ADVANCE_IMPL_H

#include <gnuradio/lfmTools/tag_advance.h>
#include <vector>

namespace gr {
namespace lfmTools {

class tag_advance_impl : public tag_advance
{
private:
    int d_advance_samples;
    std::vector<float> d_delay_buffer;
    size_t d_head; // Pointer/index for ring buffer read position

public:
    explicit tag_advance_impl(int advance_samples);
    ~tag_advance_impl() override;

    void set_advance_samples(int advance_samples) override;
    int advance_samples() const override { return d_advance_samples; }

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_TAG_ADVANCE_IMPL_H */
