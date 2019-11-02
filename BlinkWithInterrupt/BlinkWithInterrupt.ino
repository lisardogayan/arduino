// Based on https://forum.arduino.cc/index.php?topic=346731.20;wap2
/* In order to blink the built in led, it is generated a square wave with the
 * the Power Wave Modulation (PWM) of arduino in pin 3. The signal is passed 
 * to pin 7 with a wire in order to throw an interrupt that blinks pin 6.
 * 
 * IT MUST BE USED A WIRE TO JOIN PINS 3 AND 7
 */

const byte pwmPin = 3;
const byte ledPin = 6;
const byte interruptPin = 7;
volatile byte state = LOW;
volatile int count=0;

void setup() 
{ 
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);

  //---PWM CONFIGURATION
  
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(3) |          // Divide the 48MHz clock source by divisor 3: 48MHz/3=16MHz
                    GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |         // Enable GCLK4
                     GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                     GCLK_GENCTRL_ID(4);          // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Enable the port multiplexer for digital pin pwmPin (D3): timer TCC0 output
  PORT->Group[g_APinDescription[pwmPin].ulPort].PINCFG[g_APinDescription[pwmPin].ulPin].bit.PMUXEN = 1;
  
  // Connect the TCC0 timer to the port output - port pins are paired odd PMUO and even PMUXE
  // F & E specify the timers: TCC0, TCC1 and TCC2
  PORT->Group[g_APinDescription[pwmPin].ulPort].PMUX[g_APinDescription[pwmPin].ulPin >> 1].reg = PORT_PMUX_PMUXO_F;
  
  // Feed GCLK4 to TCC0 and TCC1
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TCC0 and TCC1
                     GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                     GCLK_CLKCTRL_ID_TCC0_TCC1;   // Feed GCLK4 to TCC0 and TCC1
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

  // Dual slope PWM operation: timers countinuously count up to PER register value then down 0
  REG_TCC0_WAVE |= TCC_WAVE_POL(0xF) |         // Reverse the output polarity on all TCC0 outputs
                    TCC_WAVE_WAVEGEN_DSBOTTOM;    // Setup dual slope PWM on TCC0
  while (TCC0->SYNCBUSY.bit.WAVE);               // Wait for synchronization

  // Each timer counts up to a maximum or TOP value set by the PER register,
  // this determines the frequency of the PWM operation:
  // 20000 = 50Hz, 10000 = 100Hz, 2500  = 400Hz, 1000000 = 1 Hz, 2000000 = 0.5 Hz
  REG_TCC0_PER = 2000000;      // Set the frequency of the PWM on TCC0 to 0.5Hz (A cicle ON-OFF will last 2 seconds.)
  while(TCC0->SYNCBUSY.bit.PER);

  // The CCBx register value corresponds to the pulsewidth in microseconds (us) (1 second switch on)
  REG_TCC0_CCB3 = 1000000;       // TCC0 CCB3 - center the servo on D3 
  while(TCC0->SYNCBUSY.bit.CCB3);

  // Divide the 16MHz signal by 8 giving 2MHz (0.5us) TCC0 timer tick and enable the outputs
  REG_TCC0_CTRLA |= TCC_CTRLA_PRESCALER_DIV8 |    // Divide GCLK4 by 8
                    TCC_CTRLA_ENABLE;             // Enable the TCC0 output
  while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization
  
  //---END OF PWM CONFIGURATION
}

void loop() { 
}

void blink() {
  Serial.print("Interruption triggered. ");   
  //Opcion 1: El estado sería el contrario a la salida del pin de salida
  //state = !state;
  //digitalWrite(ledPin, state); 
  //Opción 2: Leo el estado del pin de salida y se lo paso al led
  //digitalWrite(ledPin, digitalRead(interruptPin));
  state = digitalRead(interruptPin);
  digitalWrite(ledPin, state);
  Serial.print("The led is: ");  
  if (state) {
    Serial.println(" HIGH"); 
    count = count + 1;
    Serial.print("The led has blinked: ");
    Serial.print(count);
    Serial.println(" times.");
  }
  else {
    Serial.println(" LOW");
  }
    
  
}
