#ifndef INCLUDED_LFMTOOLS_TAG_DELAY_IMPL_H
#define INCLUDED_LFMTOOLS_TAG_DELAY_IMPL_H

#include <gnuradio/lfmTools/tag_delay.h>

namespace gr {
namespace lfmTools {

class tag_delay_impl : public tag_delay
{
private:
    int d_delay_samples;

public:
    explicit tag_delay_impl(int delay_samples);
    ~tag_delay_impl() override;

    void set_delay_samples(int delay_samples) override;
    int delay_samples() const override { return d_delay_samples; }

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_TAG_DELAY_IMPL_H */
