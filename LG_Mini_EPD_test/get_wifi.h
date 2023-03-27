#pragma once

#include <Arduino.h>

typedef struct wifi_config {
    String ssid;
    String psw;
} WIFI_CONFIG;

WIFI_CONFIG& get_wifi();
