#include "header.h"


struct Controller routes[] = {
    {"index.html", &handle_index, false},
    {"heap", &handle_heap, true},
    {"gpio", &handle_gpio, true},
    {"ping", &handle_ping, false},
    {"reboot", &handle_reboot, true},
    {"set-wifi", &handle_set_wifi, true},
    {"set-password", &handle_set_password, true},
    {"scan", &handle_scan, true},
};

// End pointer
Controller *routeEnd = routes + sizeof(routes) / sizeof(routes[0]);
