#include <SoftwareSerial.h>
//#include "Arduino.h"
#include "esp8266.h"

void ESP8266::updateDHT11 ( String T, String H ) {
  // 設定 ESP8266 作為 Client 端
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if (Serial.find("Error")) {
    debug.print("RECEIVED: Error\nExit1");
    return;
  }

  cmd = GET + "&field1=" + T + "&field2=" + H +"\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if (Serial.find( ">" ) ) {
    debug.print(">");
    debug.print(cmd);
    Serial.print(cmd);
  } else
    sendDebug("AT+CIPCLOSE");

  if (Serial.find("OK"))
    debug.println("RECEIVED: OK");
  else
    debug.println("RECEIVED: Error\nExit2");
}

void ESP8266::sendDebug (String cmd) {
  debug.print("SEND: ");
  debug.println(cmd);
  Serial.println(cmd);
}

boolean ESP8266::connect_wifi () {
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if (Serial.find("OK")) {
    debug.println("RECEIVED: OK");
    return true;
  } else {
    debug.println("RECEIVED: Error");
    return false;
  }

  cmd = "AT+CIPMUX=0";
  sendDebug(cmd);
  if ( Serial.find("Error")) {
    debug.print("RECEIVED: Error");
    return false;
  }
}
