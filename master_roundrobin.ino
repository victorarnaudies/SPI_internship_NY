#include <SPI.h>

#define E1 7 
#define E2 6
 
#define A1 10
#define B1 9
#define C1 8



byte token = 0 ;

void setup (void) {
   pinMode(E1,OUTPUT);  // Decoder1 control bit : E1 HIGH => Yi HIGH 
   pinMode(E2,OUTPUT);  // Decoder control bit : E2 HIGH => Yi HIGH

   pinMode(A1,OUTPUT);  // Decoder pin1
   pinMode(B1,OUTPUT);  // Decoder pin2 
   pinMode(C1,OUTPUT);  // Decoder pin3
   

   Serial.begin(115200);   //set baud rate to 115200 for usart
   digitalWrite(SS, HIGH); // disable Slave Select
   SPI.begin ();
   SPI.setClockDivider(SPI_CLOCK_DIV8); //divide the clock by 8
   digitalWrite(E1, LOW);               // disable decoder1
   digitalWrite(E2, LOW);               // disable decoder2

}

void loop (void) {
   char c;

   /*
    * Manage round robin from slave 0 to slave 7 with decoder1
    * 
    * PORTB :          PIN: D13 | D12 | D11 | D10 | D9  | D8
    *         decoder mask:  X  |  X  |  X  |  C  |  B  |  A
    * 
    * token : as token is a byte, its value is a direct mask for PORTB, 
    *         PORTB is also associated with decoder1 mask 
    */
    
   if(token < 8){
    SPI.beginTransaction();
    PORTB = token;
    delay(50);
    const char * p = "Hello, world!\r";
    digitalWrite(E1, HIGH);
    for (const char * p = "Hello, world!\r" ; c = *p; p++) 
    {
      SPI.transfer (c);  //transmit char to selected slave
      Serial.print(c);   // writen on the terinal for debug
    }
    digitalWrite(E1, LOW); // disable Slave Select
    SPI.endTransaction();
    token ++;
   }

   /*
    * Manage round robin from slave 8 to slave 15 with decoder2
    * 
    * PORTC :          PIN:  A5 |  A4 |  A3 |  A2 |  A1 | A0
    *         decoder mask:  X  |  X  |  X  |  C  |  B  |  A
    * 
    * token : as token is a byte, its value-8 is a direct mask for PORTC, 
    *         PORTB is also associated with decoder1 mask 
    */
   if(token > 7 && token < 16){
    SPI.beginTransaction();
    PORTB = token-8;
        delay(50);

    digitalWrite(E2, HIGH);
    for (const char * p = "Hello, world!\r" ; c = *p; p++) 
    {
      SPI.transfer (c);  //transmit char to selected slave
      Serial.print(c);   // writen on the terinal for debug
    }
    digitalWrite(E2, LOW); // disable Slave Select
    SPI.endTransaction();
    token ++;
   }

   /*
    * When token is > 15, token has made a complete turn, it is reset to 0
    */
    
   if(token > 15){
      token = 0;
      Serial.println(millis());
   }
}
