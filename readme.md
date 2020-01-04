# NodeMCU Wifi with Web Configration Interface


## Board Info

[Flash Layout](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#flash-layout)

### FLASH LAYOUT

The NodeMCU v3 device features a 4 MB (32 Mb) flash memory organized in sectors of 4k each. The flash memory address starts at 0x40200000.

[EEPROM Addr](https://github.com/espressif/esptool/issues/335#issuecomment-409390009)
0x405fb000 - 0x40200000

| Partition       | Address     | Size         |
|-----------------|-------------|--------------|
| Program (Code)  | 0x00000000  | 1MB          |
| Web File (SPIFS)|             | 2M, 1M Ota?  |
| Config (EEPROM) | 0x003fb000  | 4096 (1KB)   |
| Wifi Config     | ???         |              |



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
