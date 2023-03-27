#include "get_wifi.h"

#include <Preferences.h>

static const constexpr char* NVS_WIFI = "wifi";

// to be run with the real WiFi credentials once
// the next time they will be loaded from NVS flash preferences
static constexpr const char* ssid = "*******";
static constexpr const char* psw = "*******";

static WIFI_CONFIG _config { String(""), String("") };

WIFI_CONFIG& get_wifi()
{
    Preferences prefs;
    if (!prefs.begin(NVS_WIFI, true)) {
        prefs.end();
        Serial.println("wifi read prefs begin error");
        if (!prefs.begin(NVS_WIFI, false)) {
            Serial.println("wifi write prefs begin error");
            prefs.end();
            return _config;
        }
        bool result = prefs.putString("ssid", ssid) > 0;
        result = prefs.putString("psw", psw) > 0;
        prefs.end();
        if (!result) {
            Serial.println("wifi prefs put error");
            return _config;
        }
    }
    _config.ssid = prefs.getString("ssid");
    _config.psw = prefs.getString("psw");
    prefs.end();
    Serial.println(_config.ssid + "|" + _config.psw);
    if (_config.ssid.isEmpty() || _config.psw.isEmpty()) {
        Serial.println("empty wifi prefs!");
    }
    return _config;
}
