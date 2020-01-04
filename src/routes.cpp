#include "header.h"


struct Controller routes[] = {
    {"status", &handle_status, true},
    {"gpio", &handle_gpio, true},
    {"ping", &handle_ping, false},
    {"reboot", &handle_reboot, true},
    {"wifi", &handle_wifi, true},
    {"set-password", &handle_set_password, true},
    {"scan", &handle_scan, true},
};

// End pointer
Controller *routeEnd = routes + sizeof(routes) / sizeof(routes[0]);
