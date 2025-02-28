See the install.sh for the linux configureation
I also had to install this driver and this program to get the serial connection between wsl and my host pc to work
https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads (Windows Driver)
https://learn.microsoft.com/en-us/windows/wsl/connect-usb

Make sure to restart to update path for vscode

To then attach the device to wsl we need to open the device to usbipd and then attach it in the host powershell

`usbipd list`<br>
`usbipd bind --busid 1-6 `<br>
`usbipd attach --wsl --busid 1-6  `<br>


In  the linux terminal there is a process to building applications for the esp32

We know that the device is connected when we see in lsusb:<br>
`Bus 001 Device 003: ID 10c4:ea60 Silicon Labs CP210x UART Bridge`

and we can see a USB0 device in /dev

when starting a new session make sure to use 
`get_idf`


First we want to configure the project with <br>
`idf.py set-target esp32`<br>
`idf.py menuconfig`

Then we can build and flash
From the project directory<br>
`idf.py build`<br>
`idf.py -p /dev/ttyUSB0 flash`<br>

These projects have definitions from the sdkconfig, which doesn't really play well with VSCode, so I should add the follwoing line for each project (or rather I should only have one workplace for each project with this file included)
       <br><br>"dotConfig": "${workspaceFolder}/projects/blink/sdkconfig"`<br>
This is added to a configuration in the c_cpp_properties.json