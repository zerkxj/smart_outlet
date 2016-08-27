#ifndef ESP8266_H_INCLUDED
#define ESP8266_H_INCLUDED

#include "Arduino.h"

//*-- Hardware Serial
#define _baudrate 115200

//*-- Software Serial
// 
#define _rxpin 10
#define _txpin 11

//*-- IoT Information
#define SSID "B03B"
#define PASS "B03B54396"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149

// 使用 GET 傳送資料的格式
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&filed2=[data 2]...;
String GET = "GET /update?key=U7OT0B9MN9527NM4";

class ESP8266 {
  public:
    String cmd;
    ESP8266();
    void updateDHT11(String T, String H);
    void sendDebug(String cmd);
    boolean connect_wifi();
};
#endif
