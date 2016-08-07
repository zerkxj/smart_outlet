#include "Arduino.h"
#include "SPI.h"
#include "ADE7753.h"
#include "spi.cpp"

ADE7753::ADE7753() {
  Serial.println("ADE7753 initial done...");
}

void ADE7753::wr_reg (int addr, long data, int len) {
  Serial.println("Data write starts =====");
  byte write_cmd = B10000000;
  byte data_wr = B00000000;

  addr = addr | write_cmd;
  digitalWrite(SS,LOW);
  spi_transfer((char)(addr)); //send address

  //here there should be a t7 delay, however long that is
  for (int i=0; i<len; i++) {
    data_wr = byte(data >> (8*((len-1)-i)));
    Serial.print("write byte ");
    Serial.println(i);
    Serial.print("= 0x");
    Serial.println(data_wr, HEX);
    spi_transfer((char)(data_wr)); //send data byte
  }
  digitalWrite(SS,HIGH); //release chip, signal end transfer
}

long ADE7753::rd_reg (int addr, int len) {
  Serial.println("Data read starts =====");
  long data_rd = 0;
  byte buf_rd = 0;
  digitalWrite(SS,LOW);
  delayMicroseconds(1); // ADE7753, t1=50ns
  spi_transfer((char)(addr)); // send LSByte address
  delayMicroseconds(5); // ADE7753, t9=4us

  for (int i=1; i<=len; i++) {
    buf_rd = spi_transfer(0xFF); //get data byte
    Serial.print("read byte ");
    Serial.print(i);
    Serial.print("= 0x");
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
