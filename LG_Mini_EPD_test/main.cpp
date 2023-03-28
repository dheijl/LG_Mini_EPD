// Copyright (c) 2023 @dheijl (danny.heijl@telenet.be)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*
    Based on LilyGo Ink Screen Series u8g2Fonts Test originally created by Kaibin he
*/

#include <Arduino.h>
#include <time.h>

#define LILYGO_T5_V102

#include <GxEPD.h>
#include <boards.h>

#include <WiFi.h>

#include <GxGDGDEW0102T4/GxGDGDEW0102T4.h> //1.02" b/w

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxIO/GxIO.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

#include "get_config.h"

// for SPI pin definitions see e.g.:
// C:\Users\xxx\Documents\Arduino\hardware\espressif\esp32\variants\lolin32\pins_arduino.h

GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);

typedef struct pu_window {
    uint16_t box_x;
    uint16_t box_y;
    uint16_t box_w;
    uint16_t box_h;
} PU_WINDOW;

static constexpr const PU_WINDOW dw { 0, 15, 170, 20 };
static constexpr const PU_WINDOW tw { 0, 40, 170, 20 };

static struct tm old_time;
static struct tm curtime;

void showTime();
void showDate();

void setup(void)
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("setup");

    auto config = get_config();
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid.c_str(), config.psw.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        vTaskDelay(100);
    }
    Serial.println();
    Serial.println(WiFi.localIP());

    // get UTC time
    configTime(0, 0, config.ntp_server.c_str());
    struct tm tim;
    if (!getLocalTime(&tim)) {
        Serial.println("Could not obtain time info");
    }
    // configure local time
    setenv("TZ", config.tz.c_str(), 1);
    tzset();

    pinMode(POWER_ENABLE, OUTPUT);
    digitalWrite(POWER_ENABLE, HIGH);

    // init display
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init();
    display.update();
    display.setRotation(1);
    // clear ghost images
    display.fillScreen(GxEPD_BLACK);
    display.update();
    vTaskDelay(100);
    display.fillScreen(GxEPD_WHITE);
    display.update();
    vTaskDelay(100);

    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);

    Serial.println("setup done");
}

void loop()
{
    if (!getLocalTime(&curtime)) {
        Serial.println("Could not obtain time info");
    }
    if (curtime.tm_hour != old_time.tm_hour) {
        display.update();
    }
    if (curtime.tm_sec != old_time.tm_sec) {
        showTime();
        if (curtime.tm_mday != old_time.tm_mday) {
            showDate();
        }
        old_time = curtime;
    }
    vTaskDelay(10);
}

void showTime()
{
    display.fillRect(tw.box_x, tw.box_y, tw.box_w, tw.box_h, GxEPD_WHITE);
    display.setCursor(tw.box_x, tw.box_y + 16);
    display.printf("%02d:%02d:%02d", curtime.tm_hour, curtime.tm_min, curtime.tm_sec);
    display.updateWindow(tw.box_x, tw.box_y, tw.box_w, tw.box_h, true);
}

void showDate()
{
    display.fillRect(dw.box_x, dw.box_y, dw.box_w, dw.box_h, GxEPD_WHITE);
    display.setCursor(dw.box_x, dw.box_y + 16);
    display.printf("%d/%d/%d", curtime.tm_mday, curtime.tm_mon + 1, curtime.tm_year + 1900);
    display.updateWindow(dw.box_x, dw.box_y, dw.box_w, dw.box_h, true);
}
