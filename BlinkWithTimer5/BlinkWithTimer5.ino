/*Based on demo for Timer5 library    
  use TC5 of the SAMD processor (Arduino Zero and MKR1000)
  to generate timer based interrupts
  see https://github.com/michael71/Timer5
  This example code is in the public domain
*/

#include "Timer5.h"

int led = LED_BUILTIN;
volatile int count=0;

//long t = millis();    // timer variable for printout

void setup()
{   
  pinMode(led,OUTPUT);
  
  // debug output at 115200 baud
  SerialUSB.begin(115200);
  //while (!SerialUSB) ;
    
  SerialUSB.println("starting");

  // define frequency of interrupt
  MyTimer5.begin(0.5);  // 200=for toggle every 5msec // 0.5 = 2 sec

  // define the interrupt callback function
  MyTimer5.attachInterrupt(Timer5_IRQ);
  
  // start the timer
  MyTimer5.start();
}

// will be called by the MyTimer5 object
// toggles LED state at pin 10
void Timer5_IRQ(void) {
    static bool on = false;
    
    count++;  // count number of toggles
    if (on == true) {
      on = false;
        digitalWrite(led,LOW);
    } else {
      on = true;
        digitalWrite(led,HIGH);
    }
    Serial.print("The led is: "); 
    Serial.println(on? "LOW":"HIGH"); 
    if (on) {
      Serial.print("The led has toggled: ");
      Serial.print(count);
      Serial.println(" times.");
    }
}

void loop()
{
  // print every 10 secs how often the
  // timer interrupt was called
    
//  if (millis() - t >= 10000) {
//      t = millis();
//      SerialUSB.print(millis());
//      SerialUSB.print("  count=");
//      SerialUSB.println(count);
//  }  
}
