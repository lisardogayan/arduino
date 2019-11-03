/*  The program allows to switch on or off a led from the serial port
 *  If input 
 *  ON -> Switches the led on and reply a message of LED ON
 *  OFF -> Switches the led off and reply a message of LED OFF
 *  
 *  Furthermore of the requirements in the activity. The program also works
 *  with any on or off not depending of lower or upper case.
 *  
 *  Also it have been added some messages, checking if the LED is ON or OFF.
 *  
 *  References:
 *  
 *  https://www.codeproject.com/Articles/1255400/Processing-data-from-serial-line-in-Arduino
 *  https://www.norwegiancreations.com/2017/12/arduino-tutorial-serial-inputs/
 *  http://manueldelgadocrespo.blogspot.com/p/serialavailable.html
 *  https://www.instructables.com/id/Arduino-Function-Serialread-And-SerialreadString/
 *  
 */
#include <string.h>
#define   TERMINATOR_CHAR ('\r')
#define   NEWLINE_CHAR ('\n')

const int PIN = LED_BUILTIN;
int incomingByte = 0; // for incoming serial data
String input = "";

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(PIN, OUTPUT);
}

void loop() {
  // reply only when you receive data:
  while (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();    

    // if the character is a return o una linea nueva
    if ((incomingByte == TERMINATOR_CHAR) || (incomingByte == NEWLINE_CHAR)) {
      Serial.println("");
      processCommand(input);
      input = "";
      Serial.flush();
    }
    else { // say what you got:
      input += (char)incomingByte;
      Serial.print((char)incomingByte);      
    }   
  }
}

void processCommand(String cmd){
  //Serial.println("Comando redibido:" + cmd);
  cmd.toUpperCase();
  if (cmd.equals("ON")) {
      if (digitalRead(PIN) == LOW) {
        digitalWrite(PIN,HIGH);
        Serial.println("LED ON");     
      }
      else {
        Serial.println("You input ON, but LED is already ON, please input OFF to switch it off.");
      }
    }
    else if (cmd.equals("OFF")) {
      if (digitalRead(PIN) == HIGH) {
        digitalWrite(PIN,LOW);
        Serial.println("LED OFF");
      }
      else {
        Serial.println("You input OFF, but LED is already OFF, please input ON to switch it on.");
      }
    }
    else {
      if (digitalRead(PIN) == HIGH) {
        Serial.println("Currently LED is ON, please input OFF to switch it off.");
      }
      else {
        Serial.println("Currently LED is OFF, please input ON to switch it on.");
      }
      
    } 
}
