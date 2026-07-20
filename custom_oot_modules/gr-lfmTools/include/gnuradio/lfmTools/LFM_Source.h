#ifndef INCLUDED_LFMTOOLS_LFM_SOURCE_H
#define INCLUDED_LFMTOOLS_LFM_SOURCE_H

#include <gnuradio/lfmTools/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lfmTools {

class LFMTOOLS_API LFM_Source : virtual public sync_block {
public:
    typedef std::shared_ptr<LFM_Source> sptr;
    static sptr make(float bandwidth = 200e3,
                     float duty_cycle = 0.5,
                     float pri = 500e-6,
                     float samp_rate = 2e6,
                     float amplitude = 0.5);
                     
    virtual void set_bandwidth(float bandwidth) = 0;
    virtual void set_duty_cycle(float duty_cycle) = 0;
    virtual void set_pri(float pri) = 0;
    virtual void set_samp_rate(float samp_rate) = 0;
    virtual void set_amplitude(float amplitude) = 0;
};

} // namespace lfmTools
} // namespace gr

#endif /* INCLUDED_LFMTOOLS_LFM_SOURCE_H */
