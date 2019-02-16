

Programming MidiMixFix in Windows:
-----------------------------------


Three things are needed to flash upgrade the software in MidiMixFix with the contents of this zip file:

1. USBasp driver installed

2. Avrdude installed (available in WinAVR package)

3. Midimixfix in bootloader mode connected with USB cable

Then, the batch file "Flash Midimixfix(...).bat" should run successfully. 


Detailed instructions:
----------------------

1. Installing USBasp driver:

- Go to http://www.fischl.de/usbasp/  and download for driver for windows, for example: http://www.fischl.de/usbasp/usbasp-windriver.2011-05-28.zip

- Unpack zip file.

- While holding select button, connect MidiMixFix to PC with USB cable, in order to activate the USBasp device. Now Windows will discover a new USB device and may fail to install a driver for it. It this happens, go to control panel and find the non-working USB device, in properties change the driver to the one from the zip file.


2. Installing WinAVR:

- Go to http://sourceforge.net/projects/winavr/files/ and download the latest version of WinAVR. Install it.


3. A sure way to bring Midimixfix in bootloader mode is to hold down the Select button while connecting the USB cable. This will make Midimixfix start up in bootloader mode. The LCD may show two black bars in this mode.


Now execute "Flash MidiMixFix(...).bat"!



By Lars Ole Pontoppidan, 2014-01-19

