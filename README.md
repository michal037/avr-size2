# *AVR size 2*

### [Download](https://github.com/michal037/avr-size2/releases)
> If you want to thank, star this project.

## *Features*
* Customizable output function
* Memory overflow indicator
* Without specifying the device name
* Plugin ready
* Easy and Simple

## *Usage*
**avr-size2** `file` `(optional parameter)`

### Built-in usage for console
```
avr-size2 avr_app.elf

device: atmega32a
=--------------------------=
flash:      202 [B]    0.6 %
sram:        38 [B]    1.9 %
eeprom:     255 [B]   24.9 %
```

```
avr-size2 avr_app.elf

device: atmega32a
=-----|MemoryOverflow|-----=
flash:      206 [B]    0.6 %
sram:        42 [B]    2.1 %
eeprom:    1432 [B]  139.8 % <- Overflow
```

### Built-in usage for plugin
```
avr-size2 avr_app.elf plugin

atmega32a;202;0.6;38;1.9;255;24.9;
```
## *License*
**[MIT License](https://raw.githubusercontent.com/michal037/avr-size2/master/LICENSE)**

**Copyright (c) from 2017, Michal Kozakiewicz, [github.com/michal037](https://github.com/michal037)**

*Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:*

*The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.*

*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*
