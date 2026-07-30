#include "reset_tagger_impl.h"
#include <gnuradio/io_signature.h>
#include <cstring>

namespace gr {
namespace lfmTools {

reset_tagger::sptr reset_tagger::make(uint64_t wait)
{
    return gnuradio::get_initial_sptr(new reset_tagger_impl(wait));
}

reset_tagger_impl::reset_tagger_impl(uint64_t wait)
    : gr::sync_block("reset_tagger",
                     gr::io_signature::make(1, 1, sizeof(gr_complex)),
                     gr::io_signature::make(1, 1, sizeof(gr_complex))),
      d_wait(wait)
{
    message_port_register_in(pmt::mp("reset"));
    set_msg_handler(pmt::mp("reset"), [this](pmt::pmt_t msg) {
        d_pending_resets.fetch_add(1, std::memory_order_relaxed);
    });
}

int reset_tagger_impl::work(int noutput_items,
                            gr_vector_const_void_star& input_items,
                            gr_vector_void_star& output_items)
{
    // Pass data directly to output
    std::memcpy(output_items[0], input_items[0], noutput_items * sizeof(gr_complex));

    uint64_t offset_base = nitems_written(0);

    for (int i = 0; i < noutput_items; ++i) {
        // Start waiting if a reset message arrived and we aren't already counting down
        if (!d_waiting && d_pending_resets.load(std::memory_order_relaxed) > 0) {
            d_pending_resets.fetch_sub(1, std::memory_order_relaxed);
            d_waiting = true;
            d_count = 0;
        }

        if (d_waiting) {
            if (d_count == d_wait) {
                add_item_tag(0, offset_base + i, pmt::mp("reset"), pmt::PMT_T);
                d_waiting = false;
            } else {
                d_count++;
            }
        }
    }

    return noutput_items;
}

} // namespace lfmTools
} // namespace gr
