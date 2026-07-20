cd gr-lfmTools
# gr_modtool bind LFM_On_Trigger
#  gr_modtool bind LFM_Source
#  gr_modtool bind peak_detector
rm -rf build
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
