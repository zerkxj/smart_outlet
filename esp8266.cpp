#include <SoftwareSerial.h>
//#include "Arduino.h"
#include "esp8266.h"

// Constructor
ESP8266::ESP8266(Stream *s, Stream *d) : stream_pc(s), stream_esp8266(d) {
  stream_pc -> print("esp8266 constructor test ok...");
}

void ESP8266::updateDHT11 ( String T, String H ) {
  // 設定 ESP8266 作為 Client 端
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if (stream_pc -> find("Error")) {
    stream_esp8266 -> print("RECEIVED: Error\nExit1");
    return;
  }

  cmd = GET + "&field1=" + T + "&field2=" + H +"\r\n";
  stream_pc -> print("AT+CIPSEND=");
  stream_pc -> println(cmd.length());
  if (stream_pc -> find( ">" ) ) {
    stream_esp8266 -> print(">");
    stream_esp8266 -> print(cmd);
    stream_pc -> print(cmd);
  } else
    sendDebug("AT+CIPCLOSE");

  if (stream_pc -> find("OK"))
    stream_esp8266 -> println("RECEIVED: OK");
  else
    stream_esp8266 -> println("RECEIVED: Error\nExit2");
}

void ESP8266::sendDebug (String cmd) {
  stream_esp8266 -> print("SEND: ");
  stream_esp8266 -> println(cmd);
  stream_pc -> println(cmd);
}

boolean ESP8266::connect_wifi () {
  stream_pc -> println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if (stream_pc -> find("OK")) {
    stream_esp8266 -> println("RECEIVED: OK");
    return true;
  } else {
    stream_esp8266 -> println("RECEIVED: Error");
    return false;
  }

  cmd = "AT+CIPMUX=0";
  sendDebug(cmd);
  if ( stream_pc -> find("Error")) {
    stream_esp8266 -> print("RECEIVED: Error");
    return false;
  }
}
