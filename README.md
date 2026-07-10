# NREIP_Su2026_Radar_emulator

GNU Radio-based radar emulator for the NREIP Summer 2026 project.

---

## Prerequisites

Before running any flowgraphs, install the required custom GNU Radio Out-of-Tree (OOT) modules.

## Installation

From the repository root, run:

```bash
cd custom-oot-modules
./install.sh
```

This installs the custom GNU Radio modules required by the project.

---

## Running a Flowgraph

From the repository root, launch GNU Radio Companion with the desired flowgraph:

```bash
cd grc
gnuradio-companion NAME_OF_FLOWGRAPH.grc
```

Replace `NAME_OF_FLOWGRAPH.grc` with the name of the flowgraph you want to open.
