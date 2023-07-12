# PIC16F1503 slave mode
via this code, the chip will working in the I2C slave mode.
the chip will recive command (0x52 0xAD or 0x18 0xE7) form the master, then the chip will ctrl a motor via A4950.
there seems something wrong with the data jugement part, 0x52 & 0xE7 can't be read ?
the master also can inquery the state of the motor via 2 limit switch (which are cantect to pin C4&C5)
