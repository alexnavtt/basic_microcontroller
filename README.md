# Basic Microcontroller

As the name suggests, this repo contains files and code an educational project of mine to build a microcontroller 
breakout board from scratch. I picked the ATMega32u4 for its native USB status, as well as for the wealth of tutorial
and educational material out there on the web. The board itself doesn't have much of a purpose, though anyone is free 
to use it as a reference for a similar project of their own. 

The board has the following features:
  * ATMega32u4 MCU running at 8 MHz
  * USB-C terminal
  * 6-pin ICSP header terminal
  * 1x ADC screw terminal
  * 1x I2C Stemma/QWIIC header
  * 1x RGB LED
  * 5x5 green LED array

There's no particular reason that I run the board at 8 MHz. Generally this MCU is operated at 16 MHz when operating a 
5VDC (which this board is). I just kinda picked 8 on a whim, and as a result I got an additional lesson in recompiling
Arduino bootloaders with modified settings, which I'm honestly really happy with. For an educational project, more things
to learn is always a good thing. Also, placing the LED array on the opposite side but at the same location as the MCU
forced me to get a little creative with wiring, and as my first 4-layer PCB I'm happy with the result 

I have no plans to update this board in the future, but I am thiniking of doing a similar project which an ARM MCU, probably
something simple like an ESP32. We'll see where I end up.
