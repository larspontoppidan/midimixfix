@echo off
echo Connect Midimixfix to PC in bootloader mode. This can be done by holding SELECT button down as the USB cable is inserted.
pause
avrdude -pm644p -cusbasp -U flash:w:Midimixfix_v1.0.hex:i -B 1
pause
