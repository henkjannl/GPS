https://github.com/espressif/arduino-esp32/issues/272

Some interface boards for SD card have a design error. The MISO line will be at ground level if no card is inserted. 
Even if the CS is not active. The enable input of the buffer for MISO is connected to ground and not to CS.

Same issue here, using TTGO V2.1 boards. But could solve it by moving to this lib:
https://github.com/nhatuan84/esp32-micro-sdcard

It works for SD access via SPI interface, while SD.cpp from arduino-esp32 doesn't.

