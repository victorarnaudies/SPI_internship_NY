#include <SPI.h>

#define BUFF_SIZE 100

char buff[BUFF_SIZE];
unsigned int indx=0;
byte process;
byte command=0;

void setup (void) {
   Serial.begin (115200);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   Serial.println("Slave 2");
   for(int i = 0 ; i< BUFF_SIZE-1 ; i++){
    buff[i]=0x42;
   }
   buff[BUFF_SIZE-1]=0x41;
   SPCR |= _BV(SPE);
   SPCR |= !(_BV(MSTR)); //Arduino is Slave
   SPDR |= 0x42;
   SPCR |= _BV(SPIE);
   
}

ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
  byte c= SPDR;  //buff[indx++];  
  indx++;

  if(indx == 4){
    process++;
    indx=0;
  }
  /*
     byte c = SPDR;

   if( c == 0x00 && indx < 1000 ){ 
    SPDR = buff[indx++]; // save data in the next index in the array buff
   }
   if( indx == 1000){
    process++;
   }

  switch (command)
  {
  // no command? then this is the command
  case 0:
    command = c;
    break;
  case 0x01:
    command = c;
    SPDR = 0x10; //buff[indx++];  
    break;
  case 0x02:
    command = c;
    SPDR = 0x11;//buff[indx++];
    process++;  
    break;
  } // end of switch*/
  /*
  switch (command)
  {
  // no command? then this is the command
  case 0:
    command = c;
    SPDR = 0;
    break;
    
  // add to incoming byte, return result
  case 'a':
    SPDR = c + 15;  // add 15
    break;
    
  // subtract from incoming byte, return result
  case 's':
    SPDR = c - 8;  // subtract 8
    break;
  }*/

}

void loop (void) {
       SPDR = 0x42;

   if (process) {
     Serial.println(indx+1);
     process--; //reset the process
     indx= 0; //reset button to zero
     delay(2000);
   }
}
