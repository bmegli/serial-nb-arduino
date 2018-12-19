# serial-nb-arduino

Arduino/Teensy library for non-blocking serial (UART/USB) communication

## Hardware 

Tested on Teensy 3.5

## Purpose

- non-blocking serial communication (UART/USB)
- circular buffer dropping data if other side not receiving
- mostly for my internal needs

## State

Functional but in early progress. Clumsy interface needs simplification.

Currently:
- working implementation
- no circular buffer, just shift buffer
- if other side not receiving dropping actual data, not oldest
- it is caller responsibility to check if the buffer has enough space 

Both library interface and implementation may change.

## Installation

- copy or clone `serial-nb-arduino` directory to your sketchbook `libraries` directory

## Using

```C++
#include <serial-nb.h>

//Use Teensy 3.5 USB
SerialNB<usb_serial_class> serial(Serial); 

//alternatively for UART and e.g. Serial1
//SerialNB<HardwareSerial> serial(Serial1);

void setup()
{
  Serial.begin(115200);
}
  
void loop()
{
  int x=7;
  uint8_t data[4];

  //do some other non blocking stuff

  if( serial.free_bytes() >= sizeof(x) + sizeof(data) )
  {
    serial.push(x);
    serial.push(data, 4);
  }
  
  //this function doesn't block waiting for send
  //but it expects the same from the rest of your code
  serial.send();
}
```

## License

Library is licensed under Mozilla Public License, v. 2.0

This is similiar to LGPL but more permissive:

- you can use it as LGPL in prioprietrary software
- unlike LGPL you may compile it statically with your code

Like in LGPL, if you modify this library, you have to make your changes available. Making a github fork of the library with your changes satisfies those requirements perfectly.
