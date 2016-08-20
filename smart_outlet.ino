#include "ade7753.h"
#include "SPI.h"

//#define DATAOUT 51 // MOSI
//#define DATAIN 50 // MISO 
//#define SPICLOCK 52 // SCK
//#define SLAVESELECT 53 // SS

byte eeprom_output_data;
byte eeprom_input_data = 0;
long long_eeprom_data = 0;
byte clr;
int address = 0;

ADE7753 ade7753;

//data buffer
char buffer [128];

void fill_buffer () {
  for (int I=0; I<128; I++)
    buffer[I]=I;
}

void setup () {
  Serial.begin(9600);

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
  Serial.print("reg MODE = ");
  Serial.println(long_eeprom_data, HEX);

  address = LINECYC;
  long TestWrite;
  TestWrite = 0xABCD;
  Serial.print("write register LINECYC = 0x");
  Serial.println(TestWrite, HEX);
  ade7753.wr_reg(address, TestWrite, 2);
  delay(1000);
  long_eeprom_data = ade7753.rd_reg(address, 2);
  Serial.println(eeprom_output_data, HEX);
  if (TestWrite == long_eeprom_data)
    Serial.println("Completed basic write test OK...");
  else
    Serial.println("Completed basic write test Fail...");

//  attachInterrupt(2, ade7753.rd_irms, RISING); // setup ISR

  // check interrupt enable setting
  long_eeprom_data = ade7753.rd_reg(IRQEN, 2);
  Serial.print("before setup..., IREQEN = ");
  Serial.println(long_eeprom_data, HEX);

  ade7753.wr_reg(IRQEN, 0x0010, 2); // set interrupt enable for zero crossing
  long_eeprom_data = ade7753.rd_reg(IRQEN, 2); // check interrupt enable setting
  Serial.print("after setup..., IREQEN = ");
  Serial.println(long_eeprom_data, HEX);

  long_eeprom_data = ade7753.rd_reg(RSTSTATUS, 2); // reset interrupt status
  Serial.println("clear interrupt first...");
  delay(10000);

  Serial.println("initial done...");
}

void loop () {
   long_eeprom_data = ade7753.rd_reg(MODE, 2);
   Serial.println("MODE CHECK");
   Serial.println(long_eeprom_data, HEX);
   delay(1000);
}
