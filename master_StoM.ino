#include <SPI.h>

#define E1 10 
#define BUFF_SIZE 100

byte buff[BUFF_SIZE];
byte token = 0 ;
int indx = 0;

void setup (void) {
   Serial.begin(115200);   //set baud rate to 115200 for usart
   //digitalWrite(SS, HIGH); // disable Slave Select
   SPI.begin ();
   SPDR = 0;
   for(int i = 0 ; i< BUFF_SIZE ; i++){
    buff[i]=252;
   }
   delay(500);
}
int process=1;
void loop (void) {
  byte t;
  while(indx < 4){
   //SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
   digitalWrite(E1, LOW);
   SPI.transfer( buff,1);  //transmit char to selected slave
   digitalWrite(E1, HIGH); // disable Slave Select
   //SPI.endTransaction();
   indx++;
   Serial.println(buff[0]);
   process--;
  }
  for(int i = 0; i < 4 ; i++){
    Serial.println(buff[i]);
  }
  delay(1000);
  process++;
}
