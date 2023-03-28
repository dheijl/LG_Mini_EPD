#pragma once

#include <Arduino.h>

typedef struct wifi_config {
    String ssid;
    String psw;
    String ntp_server;
    String tz;
} DEV_CONFIG;

DEV_CONFIG& get_config();
