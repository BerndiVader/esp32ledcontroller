#include <iostream>
#include <sstream>

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <FastLED.h>

#define ONBOARD_LED 2
#define WIFI_SSID "ESP32"
#define WIFI_PWD "87654321"
#define UDP_PORT 19740

using namespace std;

class Helper
{

private:

    const uint8_t ip[4]={192,168,43,100};
    const uint8_t sub[4]={255,255,255,0};

    const char* handshake_msg="ESP32HNDSHK";

    void switch_buildin_led(int onoff);
    void dim_buildin_led(int brightness);
    void create_udb_server();

public:
    Helper();

    int buildin_led_brightness=255;
    bool buildin_led;

    AsyncUDP *udp;

    void update_buildin_led();
    void send_handshake_packet(AsyncUDPPacket& packet);

    bool WIFI_setup();

    enum UDP_COMMAND {
        NONE                = (int)'0',
        BUILDIN_LED_SWITCH  = (int)'1',
        BUILDIN_LED_DIMM    = (int)'2',
        RGBSTRIP_SWITCH     = (int)'3',
        RGBSTRIP_DIMM       = (int)'4',
        RGB_COUNT           = (int)'A',
        HANDSHAKE           = (int)'9'
    };

};
