cd gr-lfmTools
gr_modtool bind LFM_On_Trigger
gr_modtool bind LFM_Source
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
