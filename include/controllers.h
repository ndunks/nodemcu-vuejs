#pragma once

void handle_index(String &response, HTTPMethod method);
void handle_status(String &response, HTTPMethod method);
void handle_gpio(String &response, HTTPMethod method);
void handle_ping(String &response, HTTPMethod method);
void handle_reboot(String &response, HTTPMethod method);
void handle_wifi(String &response, HTTPMethod method);
void handle_set_password(String &response, HTTPMethod method);
void handle_scan(String &response, HTTPMethod method);
