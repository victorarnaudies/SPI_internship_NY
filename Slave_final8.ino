#include <SPI.h>

// BUFF_SIZE is not a real buffer size, represents the number of bytes to read and send
// BUFF_SIZE has to be the same for both Master and Slaves
#define BUFF_SIZE 1000 

byte to_send; // byte to send

void setup (void) {
  SPI_SlaveInit();
}


void loop (void) {
  /**
   * if slave receives 0x01 byte, it sends back BUFF_SIZE incremented to_send bytes
   * else it receives a broadcast essage from master and listen to it
   */
  if( SPI_SlaveWaitRequest() == 0x01 ){
    for(byte i = 0 ; i < BUFF_SIZE ; i ++){
      SPI_SlaveTransmit(to_send);
      to_send++;
      }
   }
   else{
    for(int i = 0 ; i < BUFF_SIZE-1 ;  i++){
      SPI_SlaveListen();
      }
    }
}

void SPI_SlaveInit(void)
{
  Serial.begin (115200);
  pinMode(MISO, OUTPUT);    // Master In Slave Out --> output for slaves only
  digitalWrite(MISO, LOW);  // set MOSI line to LOW
  to_send=0x00;             // set first byte to send to 0x00
  SPCR |= _BV(SPE);         // enable SPI
}


byte SPI_SlaveWaitRequest(void)
{
  /* Wait for reception complete */
  while(!(SPSR & (1<<SPIF)));
  return SPDR;
}

/*
 * Listen for MOSI line and updates to_send byte with the incremented value of SPDR
 * after the transmission of on byte ends
 */
byte SPI_SlaveListen(void)
{
  SPDR=0;
  /* Wait for reception complete */
  while(!(SPSR & (1<<SPIF)));
  to_send=SPDR+1;
}

/*
 * slave transmits its last to_send byte
 */
void SPI_SlaveTransmit(byte data)
{
  /* Start transmission */
  SPDR = data;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF))); 
}
