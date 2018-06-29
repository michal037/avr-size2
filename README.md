# AVR Size 2

## Features
* Customizable output function
* Memory overflow indicator
* Without specifying the device name
* Plugin ready

## Usage
```
avr-size2 file (optional-parameter)
```

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

## Reporting issues
Please feel free to [report issues](https://github.com/michal037/avr-size2/issues).

## License
This software is licensed under the MIT License. See the [license file](license.txt) for details.
