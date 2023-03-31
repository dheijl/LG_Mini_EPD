# LG_Mini_EPD (LILYGO_T5_V102)

![afbeelding](https://user-images.githubusercontent.com/2384545/228531196-47f3a692-ee10-451c-bc2d-1bc0b8e5144b.png)


Simple digital clock example with the LilyGo ESP32 mini-EPD display (LILYGO_T5_V102).

It uses SNTP to get the time and configures the correct timezone.

I used the "partial_update" example sketch as a starting point.

The Wifi/SNTP/TZ configuration is read from a file "config.txt" on the SD card if present and copied to NVS preferences.

This config file consists of 4 lines with:

- wifi ssid
- wifi password
- the ntp server
- the timezone string

So the first time you run the program it needs a valid config.txt file in the SD card slot.

If no SD card is present the Wifi/NTP/TZ configuration is read from the NVS preferences that were previously copied from the SD card.

An example config.txt is provided.

## Arduino config (vscode)

### Board: ESP32-PICO-D4

```sh
C:\\Users\\%USER%\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.7\\cores\\esp32
C:\\Users\\%USER%\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.7\\variants\\pico32
```

### Libraries

```sh
C:\\Users\\%USER%\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.7\\libraries\\FS\\src
C:\\Users\\%USER%\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.7\\libraries\\Preferences\\src
C:\\Users\\%USER%\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.7\\libraries\\SD\\src
C:\\Users\\%USER%\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.7\\libraries\\SPI\\src
C:\\Users\\%USER%\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.7\\libraries\\Wire\\src
C:\\Users\\%USER%\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.7\\libraries\\WiFi\\src"
X:\\Documents\\Arduino\\libraries\\GxEPD\\src
X:\\Documents\\Arduino\\libraries\\Adafruit_GFX_Library
X:\\Documents\\Arduino\\libraries\\Adafruit_BusIO
```

### Intellisense

For intellisense _default_ (not the tagparser) to work reliably I found that I have to:

- edit c_cpp_properties.json and
  - change gcc-x64 to gcc-x86
  - change c11 to c17 and c++11 to gnu++17
- prevent regenerating intellisense by disabling it in arduino.json

If you add headers you have to enable/regenerate the intellisense and apply these changes again.
