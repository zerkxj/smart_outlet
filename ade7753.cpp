#include "Arduino.h"
#include "SPI.h"
#include "ade7753.h"

ADE7753::ADE7753() {
  Serial.println("ADE7753 initial done...");
}

void ADE7753::wr_reg (byte addr, long data, int len) {
  Serial.println("Data write starts =====");
  byte write_cmd = B10000000;
  byte data_wr = B00000000;

  addr = addr | write_cmd;
  digitalWrite(SS,LOW);
  SPI.transfer(addr); //send address

  //here there should be a t7 delay, however long that is
  for (int i=0; i<len; i++) {
    data_wr = byte(data >> (8*((len-1)-i)));
    Serial.print("write byte ");
    Serial.println(i);
    Serial.print(" = 0x");
    Serial.println(data_wr, HEX);
    SPI.transfer(data_wr); //send data byte
  }
  digitalWrite(SS,HIGH); //release chip, signal end transfer
}

long ADE7753::rd_reg (byte addr, int len) {
  Serial.println("Data read starts =====");
  long data_rd = 0;
  byte buf_rd = 0;
  digitalWrite(SS,LOW); // slave select LOW
  delayMicroseconds(1); // ADE7753, t1=50ns
  SPI.transfer(addr); // send LSByte address
  delayMicroseconds(5); // ADE7753, t9=4us

  for (int i=1; i<=len; i++) {
    buf_rd = SPI.transfer(0xFF); //get data byte
    Serial.print("read byte ");
    Serial.print(i);
    Serial.print(" = 0x");
    Serial.println(buf_rd, HEX);
    delayMicroseconds(1); // ADE7753, t10=50ns

    data_rd = data_rd | buf_rd;
    if (i< len) {
      data_rd = data_rd <<8;
    }
  }
  Serial.print("read completed. data was --> 0x");
  Serial.println(data_rd, HEX);
  digitalWrite(SS,HIGH); // release chip, signal end transfer
  return data_rd;
}

void ADE7753::rd_irms () {
  long data_rd = 0;
  data_rd = rd_reg(IRMS, 3); // read irms
  Serial.println("IRMS CHECK = 0x");
  Serial.println(data_rd, HEX);

  data_rd = rd_reg(RSTSTATUS, 2);
  Serial.print("RSTSTATUS = 0x");
  Serial.println(data_rd, HEX);
}
