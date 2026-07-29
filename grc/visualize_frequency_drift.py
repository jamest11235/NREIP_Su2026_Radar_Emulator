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
from gnuradio import analog
from gnuradio import blocks
from gnuradio import gr
from gnuradio.filter import firdes
from gnuradio.fft import window
import sys
import signal
from PyQt5 import Qt
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import uhd
import time
from gnuradio.qtgui import Range, RangeWidget
from PyQt5 import QtCore
import numpy as np
import sip



class visualize_frequency_drift(gr.top_block, Qt.QWidget):

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

        self.settings = Qt.QSettings("GNU Radio", "visualize_frequency_drift")

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
        self.samp_rate = samp_rate = int(2e6)
        self.duty_cycle = duty_cycle = 0.1
        self.amplitude = amplitude = 1.0
        self.PRI = PRI = 0.01
        self.sine = sine = amplitude*np.exp(1j * np.pi * np.arange(int(samp_rate * PRI*duty_cycle)) / int(samp_rate * PRI*duty_cycle))
        self.delay = delay = 0
        self.padded_delayed = padded_delayed = np.pad(sine, (delay, int(samp_rate * PRI) - int(samp_rate * duty_cycle * PRI)-delay))
        self.padded = padded = np.pad(sine, (0, int(samp_rate * PRI) - int(samp_rate * duty_cycle * PRI)))
        self.bandwidth = bandwidth = 300e3

        ##################################################
        # Blocks
        ##################################################

        self._PRI_range = Range(1e-6, 1, 1e-6, 0.01, 200)
        self._PRI_win = RangeWidget(self._PRI_range, self.set_PRI, "Pulse Repetition Interval", "counter", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._PRI_win)
        self.uhd_usrp_source_0_0_0 = uhd.usrp_source(
            ",".join(('addr=192.168.110.2', '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=[0],
            ),
        )
        self.uhd_usrp_source_0_0_0.set_clock_source('internal', 0)
        self.uhd_usrp_source_0_0_0.set_samp_rate(samp_rate)
        # No synchronization enforced.

        self.uhd_usrp_source_0_0_0.set_center_freq(3e9, 0)
        self.uhd_usrp_source_0_0_0.set_antenna("RX2", 0)
        self.uhd_usrp_source_0_0_0.set_bandwidth(samp_rate, 0)
        self.uhd_usrp_source_0_0_0.set_gain(10, 0)
        self.uhd_usrp_sink_0_0_0 = uhd.usrp_sink(
            ",".join(('addr=192.168.10.5', '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=list(range(0,1)),
            ),
            "",
        )
        self.uhd_usrp_sink_0_0_0.set_clock_source('external', 0)
        self.uhd_usrp_sink_0_0_0.set_samp_rate(samp_rate)
        # No synchronization enforced.

        self.uhd_usrp_sink_0_0_0.set_center_freq(3e9, 0)
        self.uhd_usrp_sink_0_0_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0_0_0.set_bandwidth(samp_rate, 0)
        self.uhd_usrp_sink_0_0_0.set_gain(15, 0)
        self.uhd_usrp_sink_0_0 = uhd.usrp_sink(
            ",".join(('addr=192.168.110.2', '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=[1],
            ),
            "",
        )
        self.uhd_usrp_sink_0_0.set_clock_source('internal', 0)
        self.uhd_usrp_sink_0_0.set_samp_rate(samp_rate)
        # No synchronization enforced.

        self.uhd_usrp_sink_0_0.set_center_freq(3e9, 0)
        self.uhd_usrp_sink_0_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0_0.set_bandwidth(samp_rate, 0)
        self.uhd_usrp_sink_0_0.set_gain(15, 0)
        self.qtgui_time_sink_x_0_1_0 = qtgui.time_sink_c(
            (int(samp_rate * PRI)*2), #size
            samp_rate , #samp_rate
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0_1_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_1_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0_1_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_1_0.enable_tags(False)
        self.qtgui_time_sink_x_0_1_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_1_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0_1_0.enable_grid(False)
        self.qtgui_time_sink_x_0_1_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_1_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0_1_0.enable_stem_plot(False)


        labels = ['I_X310_RX', 'Q_X310_RX', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                if (i % 2 == 0):
                    self.qtgui_time_sink_x_0_1_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0_1_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0_1_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_1_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_1_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_1_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_1_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_1_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_1_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_1_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_1_0_win, 2, 0, 1, 1)
        for r in range(2, 3):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.qtgui_time_sink_x_0_0_1 = qtgui.time_sink_f(
            (int(samp_rate * PRI)*2), #size
            samp_rate , #samp_rate
            "", #name
            2, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0_0_1.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0_1.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0_0_1.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_0_1.enable_tags(True)
        self.qtgui_time_sink_x_0_0_1.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0_1.enable_autoscale(False)
        self.qtgui_time_sink_x_0_0_1.enable_grid(False)
        self.qtgui_time_sink_x_0_0_1.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_0_1.enable_control_panel(False)
        self.qtgui_time_sink_x_0_0_1.enable_stem_plot(False)


        labels = ['RX_X310_Amplitude', 'RX_X310_Phase', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0_0_1.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0_0_1.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0_1.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0_1.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0_1.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0_1.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0_1.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_0_1_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0_1.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_0_1_win, 2, 1, 1, 1)
        for r in range(2, 3):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(1, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.qtgui_time_sink_x_0_0_0_0 = qtgui.time_sink_f(
            (int(samp_rate * PRI)*2), #size
            samp_rate , #samp_rate
            "", #name
            2, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0_0_0_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0_0_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0_0_0_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_0_0_0.enable_tags(True)
        self.qtgui_time_sink_x_0_0_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0_0_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0_0_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0_0_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_0_0_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0_0_0_0.enable_stem_plot(False)


        labels = ['TX_N210_Amplitude', 'TX_N210_Phase', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0_0_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0_0_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_0_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0_0_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_0_0_0_win, 1, 1, 1, 1)
        for r in range(1, 2):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(1, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.qtgui_time_sink_x_0_0_0 = qtgui.time_sink_f(
            (int(samp_rate * PRI)*2), #size
            samp_rate , #samp_rate
            "", #name
            2, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0_0_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0_0_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_0_0.enable_tags(True)
        self.qtgui_time_sink_x_0_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_0_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0_0_0.enable_stem_plot(False)


        labels = ['TX_X310_Amplitude', 'TX_X310_Phase', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_0_0_win, 0, 1, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(1, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.qtgui_time_sink_x_0_0 = qtgui.time_sink_c(
            (int(samp_rate * PRI)*2), #size
            samp_rate , #samp_rate
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_0.enable_tags(True)
        self.qtgui_time_sink_x_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0_0.enable_stem_plot(False)


        labels = ['I_N210_TX', 'Q_N210_TX', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                if (i % 2 == 0):
                    self.qtgui_time_sink_x_0_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_0_win, 1, 0, 1, 1)
        for r in range(1, 2):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_c(
            (int(samp_rate * PRI)*2), #size
            samp_rate , #samp_rate
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)


        labels = ['I_X310_TX', 'Q_X310_TX', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                if (i % 2 == 0):
                    self.qtgui_time_sink_x_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_win, 0, 0, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self._duty_cycle_range = Range(0, 1, 0.0000001, 0.1, 200)
        self._duty_cycle_win = RangeWidget(self._duty_cycle_range, self.set_duty_cycle, "Duty Cycle", "counter", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._duty_cycle_win)
        self._delay_range = Range(0, (int(PRI * samp_rate)), 1, 0, 200)
        self._delay_win = RangeWidget(self._delay_range, self.set_delay, "delay", "counter_slider", int, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._delay_win)
        self.blocks_vector_source_x_0_1 = blocks.vector_source_c(padded_delayed, True, 1, [])
        self.blocks_vector_source_x_0 = blocks.vector_source_c(padded, True, 1, [])
        self.blocks_divide_xx_0_1 = blocks.divide_ff(1)
        self.blocks_divide_xx_0_0_0 = blocks.divide_ff(1)
        self.blocks_divide_xx_0_0 = blocks.divide_ff(1)
        self.blocks_complex_to_magphase_0_1 = blocks.complex_to_magphase(1)
        self.blocks_complex_to_magphase_0_0_0 = blocks.complex_to_magphase(1)
        self.blocks_complex_to_magphase_0_0 = blocks.complex_to_magphase(1)
        self._bandwidth_range = Range(1e3, 5e6, 1, 300e3, 200)
        self._bandwidth_win = RangeWidget(self._bandwidth_range, self.set_bandwidth, "Bandwidth", "counter", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._bandwidth_win)
        self.analog_const_source_x_0_1 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, (2*np.pi))
        self.analog_const_source_x_0_0_0_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, (2*np.pi))
        self.analog_const_source_x_0_0_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, (2*np.pi))
        self._amplitude_range = Range(0, 1.01, 0.02, 1.0, 200)
        self._amplitude_win = RangeWidget(self._amplitude_range, self.set_amplitude, "Amplitude", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._amplitude_win)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_const_source_x_0_0_0, 0), (self.blocks_divide_xx_0_0_0, 1))
        self.connect((self.analog_const_source_x_0_0_0_0, 0), (self.blocks_divide_xx_0_0, 1))
        self.connect((self.analog_const_source_x_0_1, 0), (self.blocks_divide_xx_0_1, 1))
        self.connect((self.blocks_complex_to_magphase_0_0, 1), (self.blocks_divide_xx_0_0, 0))
        self.connect((self.blocks_complex_to_magphase_0_0, 0), (self.qtgui_time_sink_x_0_0_0, 0))
        self.connect((self.blocks_complex_to_magphase_0_0_0, 1), (self.blocks_divide_xx_0_0_0, 0))
        self.connect((self.blocks_complex_to_magphase_0_0_0, 0), (self.qtgui_time_sink_x_0_0_0_0, 0))
        self.connect((self.blocks_complex_to_magphase_0_1, 1), (self.blocks_divide_xx_0_1, 0))
        self.connect((self.blocks_complex_to_magphase_0_1, 0), (self.qtgui_time_sink_x_0_0_1, 0))
        self.connect((self.blocks_divide_xx_0_0, 0), (self.qtgui_time_sink_x_0_0_0, 1))
        self.connect((self.blocks_divide_xx_0_0_0, 0), (self.qtgui_time_sink_x_0_0_0_0, 1))
        self.connect((self.blocks_divide_xx_0_1, 0), (self.qtgui_time_sink_x_0_0_1, 1))
        self.connect((self.blocks_vector_source_x_0, 0), (self.blocks_complex_to_magphase_0_0, 0))
        self.connect((self.blocks_vector_source_x_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.blocks_vector_source_x_0, 0), (self.uhd_usrp_sink_0_0, 0))
        self.connect((self.blocks_vector_source_x_0_1, 0), (self.blocks_complex_to_magphase_0_0_0, 0))
        self.connect((self.blocks_vector_source_x_0_1, 0), (self.qtgui_time_sink_x_0_0, 0))
        self.connect((self.blocks_vector_source_x_0_1, 0), (self.uhd_usrp_sink_0_0_0, 0))
        self.connect((self.uhd_usrp_source_0_0_0, 0), (self.blocks_complex_to_magphase_0_1, 0))
        self.connect((self.uhd_usrp_source_0_0_0, 0), (self.qtgui_time_sink_x_0_1_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "visualize_frequency_drift")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_padded(np.pad(self.sine, (0, int(self.samp_rate * self.PRI) - int(self.samp_rate * self.duty_cycle * self.PRI))))
        self.set_padded_delayed(np.pad(self.sine, (self.delay, int(self.samp_rate * self.PRI) - int(self.samp_rate * self.duty_cycle * self.PRI)-self.delay)))
        self.set_sine(self.amplitude*np.exp(1j * np.pi * np.arange(int(self.samp_rate * self.PRI*self.duty_cycle)) / int(self.samp_rate * self.PRI*self.duty_cycle)))
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate )
        self.qtgui_time_sink_x_0_0.set_samp_rate(self.samp_rate )
        self.qtgui_time_sink_x_0_0_0.set_samp_rate(self.samp_rate )
        self.qtgui_time_sink_x_0_0_0_0.set_samp_rate(self.samp_rate )
        self.qtgui_time_sink_x_0_0_1.set_samp_rate(self.samp_rate )
        self.qtgui_time_sink_x_0_1_0.set_samp_rate(self.samp_rate )
        self.uhd_usrp_sink_0_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_sink_0_0.set_bandwidth(self.samp_rate, 0)
        self.uhd_usrp_sink_0_0_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_sink_0_0_0.set_bandwidth(self.samp_rate, 0)
        self.uhd_usrp_source_0_0_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_source_0_0_0.set_bandwidth(self.samp_rate, 0)

    def get_duty_cycle(self):
        return self.duty_cycle

    def set_duty_cycle(self, duty_cycle):
        self.duty_cycle = duty_cycle
        self.set_padded(np.pad(self.sine, (0, int(self.samp_rate * self.PRI) - int(self.samp_rate * self.duty_cycle * self.PRI))))
        self.set_padded_delayed(np.pad(self.sine, (self.delay, int(self.samp_rate * self.PRI) - int(self.samp_rate * self.duty_cycle * self.PRI)-self.delay)))
        self.set_sine(self.amplitude*np.exp(1j * np.pi * np.arange(int(self.samp_rate * self.PRI*self.duty_cycle)) / int(self.samp_rate * self.PRI*self.duty_cycle)))

    def get_amplitude(self):
        return self.amplitude

    def set_amplitude(self, amplitude):
        self.amplitude = amplitude
        self.set_sine(self.amplitude*np.exp(1j * np.pi * np.arange(int(self.samp_rate * self.PRI*self.duty_cycle)) / int(self.samp_rate * self.PRI*self.duty_cycle)))

    def get_PRI(self):
        return self.PRI

    def set_PRI(self, PRI):
        self.PRI = PRI
        self.set_padded(np.pad(self.sine, (0, int(self.samp_rate * self.PRI) - int(self.samp_rate * self.duty_cycle * self.PRI))))
        self.set_padded_delayed(np.pad(self.sine, (self.delay, int(self.samp_rate * self.PRI) - int(self.samp_rate * self.duty_cycle * self.PRI)-self.delay)))
        self.set_sine(self.amplitude*np.exp(1j * np.pi * np.arange(int(self.samp_rate * self.PRI*self.duty_cycle)) / int(self.samp_rate * self.PRI*self.duty_cycle)))

    def get_sine(self):
        return self.sine

    def set_sine(self, sine):
        self.sine = sine
        self.set_padded(np.pad(self.sine, (0, int(self.samp_rate * self.PRI) - int(self.samp_rate * self.duty_cycle * self.PRI))))
        self.set_padded_delayed(np.pad(self.sine, (self.delay, int(self.samp_rate * self.PRI) - int(self.samp_rate * self.duty_cycle * self.PRI)-self.delay)))

    def get_delay(self):
        return self.delay

    def set_delay(self, delay):
        self.delay = delay
        self.set_padded_delayed(np.pad(self.sine, (self.delay, int(self.samp_rate * self.PRI) - int(self.samp_rate * self.duty_cycle * self.PRI)-self.delay)))

    def get_padded_delayed(self):
        return self.padded_delayed

    def set_padded_delayed(self, padded_delayed):
        self.padded_delayed = padded_delayed
        self.blocks_vector_source_x_0_1.set_data(self.padded_delayed, [])

    def get_padded(self):
        return self.padded

    def set_padded(self, padded):
        self.padded = padded
        self.blocks_vector_source_x_0.set_data(self.padded, [])

    def get_bandwidth(self):
        return self.bandwidth

    def set_bandwidth(self, bandwidth):
        self.bandwidth = bandwidth




def main(top_block_cls=visualize_frequency_drift, options=None):

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
