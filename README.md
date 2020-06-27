# AW-servo_driver
'AW-servo driver' is a driver for the 'EBP Controller' project

EbpSerial must be included (refer to https://github.com/EbpController/EbpSerial)

Protocol for transmitting data is as follow: In the 6 bit data word, bit 0 = AW position (0 = left, 1 = right), bit 1 to bit 3 = AW number (value = 0 to 5)

Protocol for receiving data is as follow: In the 6 bit data word, bit 0 = AW contol (0 = no control, 1 = control), bit 1 to bit 3 = AW number (value = 0 to 5)

(also refer to EbpSerial_protocol_flowchart.pdf, https://github.com/EbpController/EbpSerial/blob/master/EbpSerial_protocol_flowchart.pdf)
