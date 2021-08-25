#include <SPI.h>

#define BUFF_SIZE 100

byte buff[BUFF_SIZE];
unsigned int indx=0;
byte process;
byte command=0;

void setup (void) {
   Serial.begin (115200);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   Serial.println("Slave 2");
   for(int i = 0 ; i< BUFF_SIZE-1 ; i++){
    buff[i]=1;
   }
   buff[BUFF_SIZE-1]=1;
   SPDR = 4;
   SPCR |= _BV(SPE);
   SPCR |= !(_BV(MSTR)); //Arduino is Slave
   SPCR |= _BV(SPIE);
   
}

ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
  while(!(SPSR & (1<<SPIF)));
  SPDR=buff[indx];
  /*if(indx == 4){
    process++;
    indx=0;
  }*/
  indx++;
}

void loop (void) {

   if (process) {
     Serial.println(indx+1);
     process--; //reset the process
     indx= 0; //reset button to zero
     delay(2000);
   }
}
