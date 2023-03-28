# LG_Mini_EPD (LILYGO_T5_V102)

Simple digital clock example with the LilyGo ESP32 mini-EPD display (LILYGO_T5_V102).

It uses SNTP to get the time and configures the correct timezone.

I used the "partial_update" example sketch as a starting point.

The Wifi/SNTP/TZ configuration is read from SD file "config.txt" if present and copied to NVS preferences.

So the first time you run the program it needs a valid config.txt file in the SD card slot.

If no SD card if present the Wifi/NTP/TZ configuration is read from the NVS preferences that were copied from the SD card.

An example config.txt is provided.
