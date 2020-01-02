# NodeMCU Wifi with Web Configration Interface

## Board Info

[Flash Layout](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#flash-layout)

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