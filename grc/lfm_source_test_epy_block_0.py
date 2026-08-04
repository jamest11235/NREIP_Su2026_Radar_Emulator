import pmt
from gnuradio import gr


class blk(gr.basic_block):
    """
    Embedded Python Block

    Parameters:
        delay (int)
        samples_per_pri (int)

    Message Input:
        meas : PMT pair, second value is an integer

    Message Output:
        dly : PMT long
    """

    def __init__(self, delay=0, samples_per_pri=1024):
        gr.basic_block.__init__(
            self,
            name="Delay Calculator",
            in_sig=None,
            out_sig=None,
        )

        self.delay = int(delay)
        self.samples_per_pri = int(samples_per_pri)

        # Internal delay state
        self.dly = 0

        # Message ports
        self.message_port_register_in(pmt.intern("meas"))
        self.set_msg_handler(pmt.intern("meas"), self.handle_meas)

        self.message_port_register_out(pmt.intern("dly"))

    def handle_meas(self, msg):
        try:
            # Expecting a PMT pair (cons cell)
            meas = int(pmt.to_long(pmt.cdr(msg)))

            self.dly = (
                self.dly
                + self.delay
                - meas
                + self.samples_per_pri
            ) % self.samples_per_pri

            # Publish updated delay as PMT long
            self.message_port_pub(
                pmt.intern("dly"),
                pmt.from_long(self.dly)
            )

        except Exception as e:
            print("Delay Calculator error:", e)

    def general_work(self, input_items, output_items):
        # Message-only block
        return 0
