#!/bin/bash
sudo apt-get install git wget flex bison gperf python3 python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
mkdir toolchain && cd toolchain
curl https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz -o xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
tar -xvf xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
echo "export PATH=\"$HOME/Workspace/ESP32/toolchain/xtensa-esp32-elf/bin:$PATH\"" >> ~/.profile
mkdir ESP-IDF && cd ESP-IDF
git clone -b v5.2.3 --recursive https://github.com/espressif/esp-idf.git esp-idf-v5.2.3
cd esp-idf-v5.2.3
install.sh esp32
echo "alias get_idf='. $HOME/esp/esp-idf/export.sh'"
