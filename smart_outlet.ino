#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
//#include <esp8266.h>
//#include "ade7753.h"
//#include "SPI.h"

//long long_eeprom_data = 0; // use for ADE7753
//byte clr;

// pin #10 is IN from sensor (GREEN wire)
// pin #11 is OUT to sensor (WHITE wire)
SoftwareSerial serial_fingerprint(10, 11);

//ADE7753 ade7753;

Adafruit_Fingerprint fingerprint = Adafruit_Fingerprint(&serial_fingerprint);

// use one led to indicate status of fingerprint check(on: pass, off: error)
boolean led_switch = false;

void setup () {

    Serial.begin(9600); // communicate with PC

    fingerprint.begin(57600); // setup fingerprint sensor serial port data rate
    delay(500);
    if (fingerprint.verifyPassword()) { // Carlos: should be change to note 1
        Serial.println("Found fingerprint sensor!");
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        while (1);
    }
    Serial.println("Waiting for valid finger...");

  /*
    // setup SPI I/F for ADE7753
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);
    pinMode(SCK,OUTPUT);
    pinMode(SS,OUTPUT);

    digitalWrite(SS,HIGH); // disable device(ADE7753)
    // enable SPI, master mode, mode = 2'b01, speed = 2'11(lowest, 250k)
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);
    SPSR = (0<<SPI2X);
    clr=SPSR;
    clr=SPDR;
    delay(10);
    Serial.println("init  SPI complete");
    delay(1000);

    long_eeprom_data = ade7753.rd_reg(MODE, 2);
    Serial.print("reg MODE = 0x");
    Serial.println(long_eeprom_data, HEX);

    long TestWrite;
    TestWrite = 0xABCD;
    Serial.print("write register LINECYC = 0x");
    Serial.println(TestWrite, HEX);
    ade7753.wr_reg(LINECYC, TestWrite, 2);
    delay(1000);
    long_eeprom_data = ade7753.rd_reg(LINECYC, 2);
    Serial.print("read register LINECYC = 0x");
    Serial.println(long_eeprom_data, HEX);
    if (TestWrite == long_eeprom_data)
        Serial.println("Completed basic write test OK...");
    else
        Serial.println("Completed basic write test Fail...");

//    attachInterrupt(2, ade7753.rd_irms, RISING); // setup ISR

    // check interrupt enable setting
    long_eeprom_data = ade7753.rd_reg(IRQEN, 2);
    Serial.print("before setup..., IREQEN = 0x");
    Serial.println(long_eeprom_data, HEX);

    ade7753.wr_reg(IRQEN, 0x0010, 2); // set interrupt enable for zero crossing
    long_eeprom_data = ade7753.rd_reg(IRQEN, 2); // check interrupt enable setting
    Serial.print("after setup..., IREQEN = 0x");
    Serial.println(long_eeprom_data, HEX);

    long_eeprom_data = ade7753.rd_reg(RSTSTATUS, 2); // reset interrupt status
    Serial.println("clear interrupt first...");
    delay(10000);
  */

    Serial.println("initial done...");
}

void loop () {
/*
  long_eeprom_data = ade7753.rd_reg(MODE, 2);
  Serial.println("MODE CHECK");
  Serial.println(long_eeprom_data, HEX);
  delay(1000);
*/

  int id_finger_print;
  id_finger_print = fingerprint.getFingerprintIDez();
  if (id_finger_print >= 0) {
      if (led_switch)
          digitalWrite(2, LOW);   // turn the LED off (LOW is the voltage level)
      else
          digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)

      led_switch = !led_switch;
  }

  delay(1000); //don't ned to run this at full speed.
  Serial.println("1 second...");
}
