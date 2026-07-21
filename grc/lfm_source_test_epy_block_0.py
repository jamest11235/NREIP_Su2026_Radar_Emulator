"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!
"""

import numpy as np
from gnuradio import gr


class blk(gr.sync_block):  # other base classes are basic_block, decim_block, interp_block
    """Embedded Python Block example - a simple multiply const"""

    def __init__(self, file="delay.txt"):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.sync_block.__init__(
            self,
            name='Embedded Python Block',   # will show up in GRC
            in_sig=[np.byte],
            out_sig=[]
        )   
        # if an attribute with the same name as a parameter is found,
        # a callback is registered (properties work, too).
        self.file = file
        self.peaks = []
        self.counter = 0
        self.started = False

    def work(self, input_items, output_items):
        """example: multiply with constant"""
        for i in range(len(input_items[0])):
            if input_items[0][i]:
                if self.started:
                    self.peaks.append(self.counter)
                else:
                    self.started = True
                self.counter = 0
            self.counter += 1
        return len(input_items[0])
        
    def stop(self):
        with open(self.file, "w") as file:
            for item in self.peaks:
                if item < 800000:
                    file.write(f"{item}\n")
        return True
