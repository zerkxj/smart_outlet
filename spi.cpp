#include "SPI.h"

char spi_transfer (volatile char data) {
  SPDR = data; // Start the transmission

  // Wait the end of the transmission
  while (!(SPSR & (1<<SPIF))) {
  };

  return SPDR; // return the received byte
}
