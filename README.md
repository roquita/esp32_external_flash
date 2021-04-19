# esp32_qspi_external_flash
## Description
* Chip name: Winbond 25Q16bv.
* Chip size: 2MB
* Chip interface: dual or quad spi.
* Product link: [Click me](https://es.aliexpress.com/item/32828658217.html?spm=a2g0s.9042311.0.0.274263c0UWiEHI)
* IDF version: 4.1
* Module: esp32 wroom

<img src="https://github.com/roquita/esp32_qspi_external_flash/blob/master/img/winbond25q16.png" alt="portfolio_view" height="400px">

## Hardware connections
ESP32 pin     | SPI bus signal | SPI Flash pin 
--------------|----------------|----------------
GPIO23        | VMOSI          | DI
GPIO19        | VMISO          | DO
GPIO18        | VSCLK          | CLK
GPIO5         | VCS            | CMD
GPIO22        | VWP            | WP
GPIO21        | VHD            | HOLD
GND           |                | GND
VCC           |                | VCC
 
If single or dual spi is needed, WP and HOLD must be connected to VCC(3.3v).</br>
For better performance, use spi native pins.</br>

## Other supported chips
According to esp32 [documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spi_flash.html#support-for-features-of-flash-chips):
```c
Flash features of different vendors are operated in different ways and need special support. 
The fast/slow read and Dual mode (DOUT/DIO) of almost all 24-bits address flash chips are supported, 
because they don’t need any vendor-specific commands.
The Quad mode (QIO/QOUT) the following chip types are supported:
    ISSI
    GD
    MXIC
    FM
    Winbond
    XMC
    BOYA
The 32-bit address range of following chip type is supported:
    W25Q256
```
