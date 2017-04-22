#ifndef ESP8266_H_INCLUDED
#define ESP8266_H_INCLUDED

#include "Arduino.h"

#define NCTU_LAB 1

//*-- Hardware Serial
#define _baudrate 115200

//*-- Software Serial
#define _rxpin 10 // Carlos: should be changed to other pin
#define _txpin 11

//*-- IoT Information
#if NCTU_LAB
    #define SSID "B03B"
    #define PASS "B03B54396"
#else
    #define SSID "CHT6097"
    #define PASS "HN74788414"
#endif

#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149

// 使用 GET 傳送資料的格式
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&filed2=[data 2]...;
String GET = "GET /update?key=U7OT0B9MN9527NM4";

class ESP8266 {
private:
    Stream *stream_pc,
           *stream_esp8266;
public:
    String cmd;
    ESP8266(Stream *s = &Serial, Stream *d = NULL);
    void updateDHT11(String T, String H);
    void sendDebug(String cmd);
    boolean connect_wifi();
};
#endif
