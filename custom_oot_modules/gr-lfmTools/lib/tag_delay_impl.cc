#include "tag_delay_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <cstring>

namespace gr {
namespace lfmTools {

tag_delay::sptr tag_delay::make(int delay_samples)
{
    return gnuradio::get_initial_sptr(new tag_delay_impl(delay_samples));
}

tag_delay_impl::tag_delay_impl(int delay_samples)
    : gr::sync_block("tag_delay",
                     gr::io_signature::make(1, 1, sizeof(float)),
                     gr::io_signature::make(1, 1, sizeof(float))),
      d_delay_samples(std::max(0, delay_samples))
{
    // Prevent GNU Radio from automatically forwarding input tags untouched
    set_tag_propagation_policy(TPP_DONT);
}

tag_delay_impl::~tag_delay_impl() {}

void tag_delay_impl::set_delay_samples(int delay_samples)
{
    gr::thread::scoped_lock lock(d_setlock);
    d_delay_samples = std::max(0, delay_samples);
}

int tag_delay_impl::work(int noutput_items,
                         gr_vector_const_void_star& input_items,
                         gr_vector_void_star& output_items)
{
    gr::thread::scoped_lock lock(d_setlock);

    const float* in = static_cast<const float*>(input_items[0]);
    float* out = static_cast<float*>(output_items[0]);

    // 1. Pass data samples through completely untouched
    std::memcpy(out, in, noutput_items * sizeof(float));

    // 2. Fetch tags inside the current input window
    std::vector<gr::tag_t> tags;
    get_tags_in_window(tags, 0, 0, noutput_items);

    // 3. Shift tags forward into the future
    for (const auto& tag : tags) {
        // Calculate new absolute sample offset
        uint64_t new_offset = tag.offset + static_cast<uint64_t>(d_delay_samples);

        add_item_tag(0,           // Output port 0
                     new_offset,  // Delayed sample position
                     tag.key,     // PMT Key
                     tag.value,   // PMT Value
                     tag.srcid);  // PMT Source ID
    }

    return noutput_items;
}

} // namespace lfmTools
} // namespace gr
