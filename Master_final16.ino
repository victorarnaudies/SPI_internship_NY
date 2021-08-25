#include <SPI.h>

// BUFF_SIZE is not a real buffer size, represents the number of bytes to send
// BUFF_SIZE has to be the same for both Master and Slaves
#define BUFF_SIZE 1000

#define A 10          // bit 0 of decoder
#define B 9           // bit 1 of decoder
#define C 8           // bit 2 of decoder
#define E1 7           // decoder1 enable pin
#define E2 6           // decoder2 enable pin

#define BROADCAST 5   // brodcast enable pin 

byte to_send;        // byte to send
byte token = 0 ;      // byte token for round robin


void setup (void) {
   SPI_MasterInit();
   delay(200);        // delay of 100 ms, let the slaves initiate 
                      // before starting communication process
}

void loop (void) {
   /*
    * Manage round robin from slave 0 to slave 7 with decoder1
    * 
    * PORTB :          PIN: D13 | D12 | D11 | D10 | D9  | D8
    *         decoder mask:  X  |  X  |  X  |  A  |  B  |  C
    * 
    * token : as token is a byte, its value is a direct mask for PORTB, 
    *         PORTB is also associated with decoder1 and decoder2 mask 
    */
 
   if(token < 8){
    PORTB = token; // set decoder bit combination
    digitalWrite(E1, HIGH); // enable Slave Select
    SPI_TokenRequest();
    digitalWrite(E1, LOW); // disable Slave Select
    SPI_MasterBroadcast();
    token+++;
   }
   else if(7 < token && token < 16 ){
    PORTB = token-8;  // set decoder bit combination
    digitalWrite(E2, HIGH); // enable Slave Select
    SPI_TokenRequest();
    digitalWrite(E2, LOW); // disable Slave Select
    SPI_MasterBroadcast();
    token++;
   }
   else{
    token = 20;
    delay(1000);
   }
}

void SPI_MasterInit(void)
{
  pinMode(SCK, OUTPUT);   // clock pin --> output for master only
  pinMode(MOSI, OUTPUT);  // Master Out Slave In --> output for master only
  
  pinMode(A,OUTPUT);  // Decoder pin1
  pinMode(B,OUTPUT);  // Decoder pin2 
  pinMode(C,OUTPUT);  // Decoder pin3
  pinMode(E1,OUTPUT);  // Decoder1 control bit : E1 HIGH => Yi HIGH 
  pinMode(E2,OUTPUT);  // Decoder1 control bit : E1 HIGH => Yi HIGH 

  pinMode(BROADCAST,OUTPUT); // broadcast controler
  
  digitalWrite(SCK, LOW);           // set clock line to 0
  digitalWrite(MOSI, LOW);          // set MOSI line to 0
  digitalWrite(E1, LOW);            // disable decoder1
  digitalWrite(E2, LOW);            // disable decoder2
  digitalWrite(BROADCAST, HIGH);    // disable Broadcast

  setClockDivider(SPI_CLOCK_DIV2);  // set clock frequency dividing intern Arduino clock

  
  SPCR |= _BV(MSTR); // put Arduino in master mode
  SPCR |= _BV(SPE);  // SPI enable
}

/**
 * Send a 0x01 byte request to the selected slave and update to_send byte
 * 
 */
void SPI_TokenRequest(void){
    int indx = 0;
    SPI_MasterRequest(); 
    while(indx < BUFF_SIZE){
      SPI_MasterReceive();
      indx++;
    }
}

/**
 *  Send BUFF_SIZE incremented to_send bytes in broadcast
 */
void SPI_MasterBroadcast(void){
  digitalWrite(BROADCAST, LOW);
  for(int i = 0 ; i < BUFF_SIZE ;  i ++){
    SPI.transfer(to_send);  //transmit char to selected slave 
    to_send++;
  }
  digitalWrite(BROADCAST, HIGH);
}

/**
 * Send 0x01 byte by setting SPDR ( SPI Data Register) 
 * before the transmission of one byte begins
 * 
 */
byte SPI_MasterRequest(void)
{
  /* Start transmission */
  SPDR = 0x01;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)))
  ;
}

/**
 * wait for transmission to end and update to_send byte with the incremented reveived value
 */
byte SPI_MasterReceive(void)
{
  SPDR = 0x00;
  /* Wait for reception complete */
  while(!(SPSR & (1<<SPIF)))
    ;
  /* Return Data Register */
  to_send=SPDR+1;  
}

/**
 * Set clock divider fro SPI transmission 
 * Arduino Uno clock is 16MHz 
 * 
 * rate can be : SPI_CLOCK_DIV2   --> 8 MHz
 *               SPI_CLOCK_DIV4   --> 4 MHz
 *               SPI_CLOCK_DIV8   --> 2 MHz
 *               SPI_CLOCK_DIV16  --> 1 MHz
 *               SPI_CLOCK_DIV32  --> 500 kHz
 *               SPI_CLOCK_DIV64  --> 250 kHz
 *               SPI_CLOCK_DIV128 --> 125 kHz
 *               
 * PS : For perfomances measure, only dividers 2,4,8,16,32 have been used
 */
void setClockDivider(byte rate)
{
  SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
  SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | (rate & SPI_2XCLOCK_MASK);
}
