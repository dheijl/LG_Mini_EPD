#include "get_config.h"

// need SPI pins for SD card
#define LILYGO_T5_V102
#include <boards.h>

#include <FS.h>
#include <Preferences.h>
#include <SD.h>

SPIClass SDSPI(VSPI);

static const constexpr char* NVS_WIFI = "wifi";

static DEV_CONFIG _config { String(""), String(""), String(""), String("") };

DEV_CONFIG& get_config()
{

    Preferences prefs;

    // check for an SD card with config info file
    SDSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI);
    if (SD.begin(SDCARD_CS, SDSPI)) {
        Serial.println("Found SD card - reading config");
        auto file = SD.open(String("/config.txt"), FILE_READ);
        if (file) {
            _config.ssid = file.readStringUntil('\n');
            _config.ssid.trim();
            _config.psw = file.readStringUntil('\n');
            _config.psw.trim();
            _config.ntp_server = file.readStringUntil('\n');
            _config.ntp_server.trim();
            _config.tz = file.readStringUntil('\n');
            _config.tz.trim();
            file.close();
            Serial.println(_config.ssid + ":" + _config.psw + ":" + _config.ntp_server + ":" + _config.tz);
            Serial.println("Saving config to NVS preferences");
            if (!prefs.begin(NVS_WIFI, false)) {
                Serial.println("wifi write prefs begin error");
                prefs.end();
            } else {
                Serial.println("Initializing config");
            }
            prefs.clear();
            bool result = true;
            if (!prefs.putString("ssid", _config.ssid) > 0) {
                result = false;
            }
            if (!prefs.putString("psw", _config.psw) > 0) {
                result = false;
            }
            if (!prefs.putString("ntp_server", _config.ntp_server) > 0) {
                result = false;
            }
            if (!prefs.putString("tz", _config.tz) > 0) {
                result = false;
            }
            prefs.end();
            if (!result) {
                Serial.println("wifi prefs put error");
            } else {
                Serial.println("config initialized");
            }
        } else {
            Serial.println("Can not find a config file!");
        }
    }
    // now read config from NVS prefs
    if (!prefs.begin(NVS_WIFI, true)) {
        prefs.end();
        Serial.println("wifi read prefs begin error");
        return _config;
    }
    _config.ssid = prefs.getString("ssid");
    _config.psw = prefs.getString("psw");
    _config.ntp_server = prefs.getString("ntp_server");
    _config.tz = prefs.getString("tz");
    prefs.end();
    Serial.println("NVS Prefs: " + _config.ssid + ":" + _config.psw + ":" + _config.ntp_server + ":" + _config.tz);
    if (_config.ssid.isEmpty() || _config.psw.isEmpty()) {
        Serial.println("empty wifi prefs!");
    }
    return _config;
}
