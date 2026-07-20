#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
# Author: jtang
# GNU Radio version: 3.10.6.0

from packaging.version import Version as StrictVersion
from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio import blocks
from gnuradio import filter
from gnuradio.filter import firdes
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from PyQt5 import Qt
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import lfmTools
from gnuradio import uhd
import time
from gnuradio.qtgui import Range, RangeWidget
from PyQt5 import QtCore
import lfm_source_test_epy_block_0 as epy_block_0  # embedded python block
import numpy as np



class lfm_source_test(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Not titled yet", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Not titled yet")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except BaseException as exc:
            print(f"Qt GUI: Could not set Icon: {str(exc)}", file=sys.stderr)
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "lfm_source_test")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except BaseException as exc:
            print(f"Qt GUI: Could not restore geometry: {str(exc)}", file=sys.stderr)

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 2e6
        self.PRI = PRI = 0.5
        self.samples_per_pri = samples_per_pri = int(PRI * samp_rate)
        self.duty_cycle = duty_cycle = 0.0005
        self.bandwidth = bandwidth = 200e3
        self.t_signal = t_signal = np.arange(samples_per_pri) / samp_rate
        self.samples_per_pulse = samples_per_pulse = int(PRI * samp_rate * duty_cycle)
        self.ramp_rate = ramp_rate = bandwidth / (PRI * duty_cycle)
        self.raw_filter = raw_filter = np.exp(-ramp_rate*np.pi*(t_signal[:samples_per_pulse]**2)*1.j)[::-1]
        self.start_freq = start_freq = 1.9e9
        self.filter_energy = filter_energy = np.sum(np.abs(raw_filter)**2)
        self.amplitude = amplitude = 0.5

        ##################################################
        # Blocks
        ##################################################

        self._duty_cycle_range = Range(0.0001, 0.001, 0.0001, 0.0005, 200)
        self._duty_cycle_win = RangeWidget(self._duty_cycle_range, self.set_duty_cycle, "Duty Cycle", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._duty_cycle_win)
        self._bandwidth_range = Range(1e3, 5e6, 1, 200e3, 200)
        self._bandwidth_win = RangeWidget(self._bandwidth_range, self.set_bandwidth, "Bandwidth", "counter", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._bandwidth_win)
        self._amplitude_range = Range(0, 1.01, 0.02, 0.5, 200)
        self._amplitude_win = RangeWidget(self._amplitude_range, self.set_amplitude, "Amplitude", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._amplitude_win)
        self._PRI_range = Range(1e-6, 1, 1e-6, 0.5, 200)
        self._PRI_win = RangeWidget(self._PRI_range, self.set_PRI, "Pulse Repetition Interval", "counter", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._PRI_win)
        self.uhd_usrp_source_0_0 = uhd.usrp_source(
            ",".join(('addr=192.168.110.2', '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=[0],
            ),
        )
        self.uhd_usrp_source_0_0.set_samp_rate(samp_rate)
        # No synchronization enforced.

        self.uhd_usrp_source_0_0.set_center_freq(start_freq, 0)
        self.uhd_usrp_source_0_0.set_antenna("RX2", 0)
        self.uhd_usrp_source_0_0.set_bandwidth(samp_rate, 0)
        self.uhd_usrp_source_0_0.set_gain(10, 0)
        self.uhd_usrp_source_0 = uhd.usrp_source(
            ",".join(('addr=192.168.10.5', '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=list(range(0,1)),
            ),
        )
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        # No synchronization enforced.

        self.uhd_usrp_source_0.set_center_freq(start_freq, 0)
        self.uhd_usrp_source_0.set_antenna("RX2", 0)
        self.uhd_usrp_source_0.set_bandwidth(samp_rate, 0)
        self.uhd_usrp_source_0.set_gain(10, 0)
        self.uhd_usrp_sink_0_0 = uhd.usrp_sink(
            ",".join(('addr=192.168.10.5', '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=list(range(0,1)),
            ),
            "",
        )
        self.uhd_usrp_sink_0_0.set_samp_rate(samp_rate)
        # No synchronization enforced.

        self.uhd_usrp_sink_0_0.set_center_freq(start_freq, 0)
        self.uhd_usrp_sink_0_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0_0.set_bandwidth(samp_rate, 0)
        self.uhd_usrp_sink_0_0.set_gain(10, 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
            ",".join(('addr=192.168.110.2', '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=[1],
            ),
            "",
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        # No synchronization enforced.

        self.uhd_usrp_sink_0.set_center_freq(start_freq, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0.set_bandwidth(samp_rate, 0)
        self.uhd_usrp_sink_0.set_gain(10, 0)
        self.lfmTools_peak_detector_0_0 = lfmTools.peak_detector(0.01, (int(PRI * duty_cycle * samp_rate)))
        self.lfmTools_peak_detector_0 = lfmTools.peak_detector(0.1, (int(PRI * duty_cycle * samp_rate)))
        self.lfmTools_LFM_Source_0 = lfmTools.LFM_Source(bandwidth, duty_cycle, PRI, samp_rate, amplitude)
        self.lfmTools_LFM_On_Trigger_0 = lfmTools.LFM_On_Trigger(bandwidth, PRI * duty_cycle, samp_rate, amplitude)
        self.fft_filter_xxx_0_0 = filter.fft_filter_ccc(1, raw_filter / filter_energy, 1)
        self.fft_filter_xxx_0_0.declare_sample_delay(0)
        self.fft_filter_xxx_0 = filter.fft_filter_ccc(1, raw_filter / filter_energy, 1)
        self.fft_filter_xxx_0.declare_sample_delay(0)
        self.epy_block_0 = epy_block_0.blk(file="delay.txt")
        self.blocks_complex_to_mag_1 = blocks.complex_to_mag(1)
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(1)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_complex_to_mag_0, 0), (self.lfmTools_peak_detector_0, 0))
        self.connect((self.blocks_complex_to_mag_1, 0), (self.lfmTools_peak_detector_0_0, 0))
        self.connect((self.fft_filter_xxx_0, 0), (self.blocks_complex_to_mag_0, 0))
        self.connect((self.fft_filter_xxx_0_0, 0), (self.blocks_complex_to_mag_1, 0))
        self.connect((self.lfmTools_LFM_On_Trigger_0, 0), (self.uhd_usrp_sink_0_0, 0))
        self.connect((self.lfmTools_LFM_Source_0, 0), (self.uhd_usrp_sink_0, 0))
        self.connect((self.lfmTools_peak_detector_0, 0), (self.lfmTools_LFM_On_Trigger_0, 0))
        self.connect((self.lfmTools_peak_detector_0_0, 0), (self.epy_block_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.fft_filter_xxx_0, 0))
        self.connect((self.uhd_usrp_source_0_0, 0), (self.fft_filter_xxx_0_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "lfm_source_test")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samples_per_pri(int(self.PRI * self.samp_rate))
        self.set_samples_per_pulse(int(self.PRI * self.samp_rate * self.duty_cycle))
        self.set_t_signal(np.arange(self.samples_per_pri) / self.samp_rate)
        self.lfmTools_LFM_On_Trigger_0.set_samp_rate(self.samp_rate)
        self.lfmTools_LFM_Source_0.set_samp_rate(self.samp_rate)
        self.lfmTools_peak_detector_0.set_look_ahead((int(self.PRI * self.duty_cycle * self.samp_rate)))
        self.lfmTools_peak_detector_0_0.set_look_ahead((int(self.PRI * self.duty_cycle * self.samp_rate)))
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_sink_0.set_bandwidth(self.samp_rate, 0)
        self.uhd_usrp_sink_0_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_sink_0_0.set_bandwidth(self.samp_rate, 0)
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_source_0.set_bandwidth(self.samp_rate, 0)
        self.uhd_usrp_source_0_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_source_0_0.set_bandwidth(self.samp_rate, 0)

    def get_PRI(self):
        return self.PRI

    def set_PRI(self, PRI):
        self.PRI = PRI
        self.set_ramp_rate(self.bandwidth / (self.PRI * self.duty_cycle))
        self.set_samples_per_pri(int(self.PRI * self.samp_rate))
        self.set_samples_per_pulse(int(self.PRI * self.samp_rate * self.duty_cycle))
        self.lfmTools_LFM_On_Trigger_0.set_pulse_width(self.PRI * self.duty_cycle)
        self.lfmTools_LFM_Source_0.set_pri(self.PRI)
        self.lfmTools_peak_detector_0.set_look_ahead((int(self.PRI * self.duty_cycle * self.samp_rate)))
        self.lfmTools_peak_detector_0_0.set_look_ahead((int(self.PRI * self.duty_cycle * self.samp_rate)))

    def get_samples_per_pri(self):
        return self.samples_per_pri

    def set_samples_per_pri(self, samples_per_pri):
        self.samples_per_pri = samples_per_pri
        self.set_t_signal(np.arange(self.samples_per_pri) / self.samp_rate)

    def get_duty_cycle(self):
        return self.duty_cycle

    def set_duty_cycle(self, duty_cycle):
        self.duty_cycle = duty_cycle
        self.set_ramp_rate(self.bandwidth / (self.PRI * self.duty_cycle))
        self.set_samples_per_pulse(int(self.PRI * self.samp_rate * self.duty_cycle))
        self.lfmTools_LFM_On_Trigger_0.set_pulse_width(self.PRI * self.duty_cycle)
        self.lfmTools_LFM_Source_0.set_duty_cycle(self.duty_cycle)
        self.lfmTools_peak_detector_0.set_look_ahead((int(self.PRI * self.duty_cycle * self.samp_rate)))
        self.lfmTools_peak_detector_0_0.set_look_ahead((int(self.PRI * self.duty_cycle * self.samp_rate)))

    def get_bandwidth(self):
        return self.bandwidth

    def set_bandwidth(self, bandwidth):
        self.bandwidth = bandwidth
        self.set_ramp_rate(self.bandwidth / (self.PRI * self.duty_cycle))
        self.lfmTools_LFM_On_Trigger_0.set_bandwidth(self.bandwidth)
        self.lfmTools_LFM_Source_0.set_bandwidth(self.bandwidth)

    def get_t_signal(self):
        return self.t_signal

    def set_t_signal(self, t_signal):
        self.t_signal = t_signal
        self.set_raw_filter(np.exp(-self.ramp_rate*np.pi*(self.t_signal[:self.samples_per_pulse]**2)*1.j)[::-1])

    def get_samples_per_pulse(self):
        return self.samples_per_pulse

    def set_samples_per_pulse(self, samples_per_pulse):
        self.samples_per_pulse = samples_per_pulse
        self.set_raw_filter(np.exp(-self.ramp_rate*np.pi*(self.t_signal[:self.samples_per_pulse]**2)*1.j)[::-1])

    def get_ramp_rate(self):
        return self.ramp_rate

    def set_ramp_rate(self, ramp_rate):
        self.ramp_rate = ramp_rate
        self.set_raw_filter(np.exp(-self.ramp_rate*np.pi*(self.t_signal[:self.samples_per_pulse]**2)*1.j)[::-1])

    def get_raw_filter(self):
        return self.raw_filter

    def set_raw_filter(self, raw_filter):
        self.raw_filter = raw_filter
        self.set_filter_energy(np.sum(np.abs(self.raw_filter)**2))
        self.fft_filter_xxx_0.set_taps(self.raw_filter / self.filter_energy)
        self.fft_filter_xxx_0_0.set_taps(self.raw_filter / self.filter_energy)

    def get_start_freq(self):
        return self.start_freq

    def set_start_freq(self, start_freq):
        self.start_freq = start_freq
        self.uhd_usrp_sink_0.set_center_freq(self.start_freq, 0)
        self.uhd_usrp_sink_0_0.set_center_freq(self.start_freq, 0)
        self.uhd_usrp_source_0.set_center_freq(self.start_freq, 0)
        self.uhd_usrp_source_0_0.set_center_freq(self.start_freq, 0)

    def get_filter_energy(self):
        return self.filter_energy

    def set_filter_energy(self, filter_energy):
        self.filter_energy = filter_energy
        self.fft_filter_xxx_0.set_taps(self.raw_filter / self.filter_energy)
        self.fft_filter_xxx_0_0.set_taps(self.raw_filter / self.filter_energy)

    def get_amplitude(self):
        return self.amplitude

    def set_amplitude(self, amplitude):
        self.amplitude = amplitude
        self.lfmTools_LFM_On_Trigger_0.set_amplitude(self.amplitude)
        self.lfmTools_LFM_Source_0.set_amplitude(self.amplitude)




def main(top_block_cls=lfm_source_test, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
