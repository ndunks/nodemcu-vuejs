# NodeMCU Wifi with Web Configration Interface


## Board Info

[Flash Layout](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#flash-layout)

### FLASH LAYOUT

The NodeMCU v3 device features a 4 MB (32 Mb) flash memory organized in sectors of 4k each. The flash memory address starts at 0x40200000.

[EEPROM Addr](https://github.com/espressif/esptool/issues/335#issuecomment-409390009)
0x405fb000 - 0x40200000

LD Config:
https://github.com/esp8266/Arduino/blob/master/tools/sdk/ld/eagle.flash.4m2m.ld

```SPI_FLASH_SEC_SIZE = 4096 = 4KB```

** Flash Split for 4M chips **

| Name   | INFO             | Mem Offset | Flash Address | Size        |
|--------|------------------|------------|---------------|-------------|
| sketch | Program (Code)   | 0x40200000 | 0x00000000    | (~1019KB) (1044464B) |
| empty  | OTA?             | 0x402FEFF0 | 0x000feff0    | (~1028KB) (1052688B) |
| spiffs | Web File         | 0x40400000 | 0x00200000    | (~2024KB) (2072576B) |
| eeprom | User Data        | 0x405FB000 | 0x003fb000    | (4KB) |
| rfcal  | SDK              | 0x405FC000 | 0x003fc000    | (4KB) |
| wifi   | SDK Wifi Config  | 0x405FD000 | 0x003fd000    | (12KB) |

** May usefull sometime**
```FREE_SPACE_AFTER_SKETCH=(ESP.getSketchSize() + FLASH_SECTOR_SIZE - 1) & (~(FLASH_SECTOR_SIZE - 1));```

## Some Code for debug
``` cpp
static const char *const status_text[] = {
    "IDLE",
    "CONNECTING",
    "WRONG_PASSWORD",
    "NO_AP_FOUND",
    "CONNECT_FAIL",
    "GOT_IP"};
static const char *const mode_text[] = {
    "OFF", "STA", "AP", "AP_STA",
    //these two pseudo modes are experimental
    "SHUTDOWN", "", "", "", "RESUME"};
```

## WEB FRONTEND

### Project setup
```
yarn install
```

### Compiles and hot-reloads for development
```
yarn serve
```

### Compiles and minifies for production
```
yarn build
```

### Upload to Device SPIFS
```
yarn deploy
```
    If not work, use platformio menu on your IDE(Upload File System Image)

## TODO

[ ] Scan And Connect to WiFi
[ ] CaptivePortal Detection (may with custom handler)
[ ] Self reset(Erase config and wifi config)


