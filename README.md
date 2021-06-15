# AW-servo_driver
'AW-servo driver' is an Arduino Uno/Nano turnout (switch) driver for the 'EBP Controller' project

In 'AW-servo_driver.ino', it's posible to modify the 'sweep time' (default=1000ms) and 'sweep angle' (default=90°) by changing the values 'SWEEP_TIME' and 'SWEEP_ANGLE'

EbpSerial must be included (refer to https://github.com/EbpController/EbpSerial)

The 10-bit address is composed as follow: 0b0001 (fixed value) + reversed value of port C bit 0 to bit 5 (e.g. pin A0 to GND makes address = 65)

Protocol for transmitting data is as follow: In the 6 bit data word, bit 0 = AW position (0 = left, 1 = right), bit 1 to bit 3 = AW number (value = 0 to 5)

Protocol for receiving data is as follow: In the 6 bit data word, bit 0 = AW contol (0 = no control, 1 = control), bit 1 to bit 3 = AW number (value = 0 to 5)

(also refer to EbpSerial_protocol_flowchart.pdf, https://github.com/EbpController/EbpSerial/blob/master/EbpSerial_protocol_flowchart.pdf)
