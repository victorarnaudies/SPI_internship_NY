#include <SPI.h>

#define E1 10 
#define BUFF_SIZE 100



char buff[BUFF_SIZE];


byte token = 0 ;

void setup (void) {
   Serial.begin(115200);   //set baud rate to 115200 for usart
   digitalWrite(SS, HIGH); // disable Slave Select
   SPI.begin ();
   digitalWrite(E1, LOW);               // disable decoder1
   for(int i = 0 ; i< BUFF_SIZE-1 ; i++){
    buff[i]=0x80;
   }
    buff[BUFF_SIZE-1]=0x02;
   delay(500);
}
int process=1;
void loop (void) {
  while(indx < 4){
   SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
   digitalWrite(E1, LOW);
   SPI.transfer(buff,4);  //transmit char to selected slave
   digitalWrite(E1, HIGH); // disable Slave Select
   SPI.endTransaction();
   int t = millis();
   Serial.println(t);
   process--;
  }
  for(int i = 0; i < 4 ; i++){
    Serial.println(buff[i]);
  }
   delay(1000);
   process++;
}
