`include "ADE7753.h"

#define DATAOUT 51 // MOSI
#define DATAIN 50 // MISO 
#define SPICLOCK 52 // SCK
#define SLAVESELECT 53 // SS

byte eeprom_output_data;
byte eeprom_input_data = 0;
long long_eeprom_data = 0;
byte clr;
int address = 0;

//data buffer
char buffer [128];

void fill_buffer () {
  for (int I=0; I<128; I++)
    buffer[I]=I;
}

char spi_transfer(volatile char data) {
  SPDR = data; // Start the transmission

  // Wait the end of the transmission
  while (!(SPSR & (1<<SPIF))) {
  };

  return SPDR; // return the received byte
}

void rd_irms () {
   long_eeprom_data = read_eeprom(IRMS, 3); // read irms
   Serial.println("IRMS CHECK");
   Serial.println(long_eeprom_data, HEX);
   
  long_eeprom_data = read_eeprom(RSTSTATUS, 2);
  Serial.print("RSTSTATUS = ");
  Serial.println(eeprom_output_data, HEX);
}

void setup () {
  Serial.begin(9600);

  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK,OUTPUT);
  pinMode(SLAVESELECT,OUTPUT);

  digitalWrite(SLAVESELECT,HIGH); // disable device(ADE7753)
  // enable SPI, master mode, mode = 2'b01, speed = 2'11(lowest, 250k)
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);
  SPSR = (0<<SPI2X);
  clr=SPSR;
  clr=SPDR;
  delay(10);
  Serial.println("init  SPI complete");
  delay(1000);


//  address = LINECYC;
  long_eeprom_data = read_eeprom(MODE, 2);
  Serial.println("reg MODE = %x", long_eeprom_data);

  long TestWrite;
  TestWrite = 0xABCD;
  Serial.println(TestWrite, BIN);
  write_to_eeprom(address, TestWrite, 2);
  eeprom_output_data = read_eeprom(address, 2);
  Serial.println(eeprom_output_data, BIN);
  Serial.println("Completed basic read write test");

  attachInterrupt(2, rd_irms, RISING); // setup ISR

  // check interrupt enable setting
  long_eeprom_data = read_eeprom(IRQEN, 2);
  Serial.print("before setup..., IREQEN = ");
  Serial.println(long_eeprom_data, HEX);

  write_to_eeprom(IRQEN, 0x0010, 2); // set interrupt enable for zero crossing
  long_eeprom_data = read_eeprom(IRQEN, 2); // check interrupt enable setting
  Serial.print("after setup..., IREQEN = ");
  Serial.println(long_eeprom_data, HEX);

  long_eeprom_data = read_eeprom(RSTSTATUS, 2); // reset interrupt status
  Serial.println("clear interrupt first...");
  delay(10000);

  Serial.println("initial done...");
}

void write_to_eeprom (int EEPROM_address, long write_buffer, int bytes_to_write) {
  byte make_write_cmd = B10000000;
  byte this_write = B00000000;
  EEPROM_address = EEPROM_address|make_write_cmd;
  digitalWrite(SLAVESELECT,LOW);
  spi_transfer((char)(EEPROM_address)); //send address

  //here there should be a t7 delay, however long that is
  for (int i=0; i<bytes_to_write; i++) {
  //Serial.println(i);
  this_write = byte(write_buffer >> (8*((bytes_to_write-1)-i)));
  //Serial.println(this_write, HEX);
  spi_transfer((char)(this_write));      //send data byte
  }
  digitalWrite(SLAVESELECT,HIGH); //release chip, signal end transfer
}

long read_eeprom(int EEPROM_address, int bytes_to_read)
{
  Serial.println("Data read starts=====");
  long data = 0;
  byte reader_buf = 0;
  digitalWrite(SLAVESELECT,LOW);
  spi_transfer((char)(EEPROM_address)); // send LSByte address

  for (int i=1; i <= bytes_to_read; i++) {
    reader_buf = spi_transfer(0xFF); //get data byte
    Serial.println(i);
    Serial.println(reader_buf, HEX);

    data = data|reader_buf;
    if (i< bytes_to_read) {
      data = data<<8;
    }
  }
  Serial.print("completed. data was>");
  Serial.println(data, HEX);
  digitalWrite(SLAVESELECT,HIGH); // release chip, signal end transfer
  return data;
}

void loop () {
/*
   eeprom_output_data = read_eeprom(STATUS,2);
   Serial.println("STATUS CHECK");
   Serial.println(eeprom_output_data, BIN);
   Serial.println(eeprom_output_data, HEX);
   delay(1000);
   eeprom_output_data = read_eeprom(LINECYC,2);
   Serial.println("LINECYC CHECK");
   Serial.println(eeprom_output_data, BIN);
   Serial.println(eeprom_output_data, HEX);   

  delay(1000);
   eeprom_output_data = read_eeprom(TEMP,1);
   Serial.println("TEMP CHECK");
   Serial.println(eeprom_output_data, BIN);
   Serial.println(eeprom_output_data, HEX);   

   delay(1000);
   eeprom_output_data = read_eeprom(IRMS,3);
   Serial.println("IRMS CHECK");
   Serial.println(eeprom_output_data, BIN);
   Serial.println(eeprom_output_data, HEX); 
 */
   long_eeprom_data = read_eeprom(MODE, 2);
   Serial.println("MODE CHECK");
   Serial.println(long_eeprom_data, HEX);
   delay(5000);
}
