#include "tag_advance_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <cstring>

namespace gr {
namespace lfmTools {

tag_advance::sptr tag_advance::make(int advance_samples)
{
    return gnuradio::get_initial_sptr(new tag_advance_impl(advance_samples));
}

tag_advance_impl::tag_advance_impl(int advance_samples)
    : gr::sync_block("tag_advance",
                     gr::io_signature::make(1, 1, sizeof(float)),
                     gr::io_signature::make(1, 1, sizeof(float))),
      d_advance_samples(std::max(0, advance_samples)),
      d_delay_buffer(d_advance_samples, 0.0f),
      d_head(0)
{
    set_tag_propagation_policy(TPP_DONT);
}

tag_advance_impl::~tag_advance_impl() {}

void tag_advance_impl::set_advance_samples(int advance_samples)
{
    gr::thread::scoped_lock lock(d_setlock);

    int new_advance = std::max(0, advance_samples);
    if (new_advance == d_advance_samples) {
        return;
    }

    if (new_advance > d_advance_samples) {
        // EDGE CASE 1: Expanding delay—re-align existing samples and zero-fill new slots
        std::vector<float> new_buffer(new_advance, 0.0f);
        
        for (int i = 0; i < d_advance_samples; ++i) {
            new_buffer[new_advance - d_advance_samples + i] = 
                d_delay_buffer[(d_head + i) % d_advance_samples];
        }
        
        d_delay_buffer = std::move(new_buffer);
        d_head = 0;
    } else if (new_advance > 0) {
        // EDGE CASE 2: Shrinking delay—keep the newest N samples, discard the oldest
        std::vector<float> new_buffer(new_advance, 0.0f);
        int discard_count = d_advance_samples - new_advance;
        
        for (int i = 0; i < new_advance; ++i) {
            new_buffer[i] = d_delay_buffer[(d_head + discard_count + i) % d_advance_samples];
        }

        d_delay_buffer = std::move(new_buffer);
        d_head = 0;
    } else {
        // Delay set to 0
        d_delay_buffer.clear();
        d_head = 0;
    }

    d_advance_samples = new_advance;
}

int tag_advance_impl::work(int noutput_items,
                          gr_vector_const_void_star& input_items,
                          gr_vector_void_star& output_items)
{
    gr::thread::scoped_lock lock(d_setlock);

    // Cast directly to float pointers
    const float* in = static_cast<const float*>(input_items[0]);
    float* out = static_cast<float*>(output_items[0]);

    // 1. Emit tags immediately at current output position
    std::vector<gr::tag_t> tags;
    get_tags_in_window(tags, 0, 0, noutput_items);

    uint64_t nwritten = nitems_written(0);

    for (const auto& tag : tags) {
        uint64_t relative_input_offset = tag.offset - nitems_read(0);
        uint64_t new_offset = nwritten + relative_input_offset;

        add_item_tag(0, new_offset, tag.key, tag.value, tag.srcid);
    }

    // EDGE CASE 3: Pass-through mode when advance_samples is 0
    if (d_advance_samples == 0) {
        std::memcpy(out, in, noutput_items * sizeof(float));
        return noutput_items;
    }

    // 2. Efficient O(1) ring buffer access instead of memory sliding
    for (int i = 0; i < noutput_items; ++i) {
        // Output oldest sample at current head position
        out[i] = d_delay_buffer[d_head];

        // Overwrite head position with fresh incoming sample
        d_delay_buffer[d_head] = in[i];

        // Increment ring index
        d_head = (d_head + 1) % d_advance_samples;
    }

    return noutput_items;
}

} // namespace lfmTools
} // namespace gr
