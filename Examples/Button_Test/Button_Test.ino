/*******************************************************************************************
 *
 * ButtonInterrupt example sketch debounces a button connected to pin 2.
 * The pushbutton should be connected between GND and pin 2.
 * See StreamingW.h for information on this library.
 *
 *******************************************************************************************
 * License:
 * This file is part of the ButtonInterrupt library.
 *
 * ButtonInterrupt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * ButtonInterrupt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public in
 * ../ButtonInterrupt/extras/gpl-3.0.txt. If not, see:
 * <http://www.gnu.org/licenses/licenses.html>.
 * 
 *******************************************************************************************
 */
 

#include <ButtonInterrupt.h>

#define SERIAL_DEBUG true
#include <SerialDebug.h>
#define SERIAL_DEBUG_SEPARATOR ", "

const byte          VER_MAJOR       =     1;    // Major version.
const byte          VER_MINOR       =     1;    // Minor version.
const byte          LED_PIN         =    13;    // LED pin.
const byte          INT_PIN         =     2;    // Pushbutton pin.
const byte          MAX_PUSH        =     5;    // Push count to temporarily disable interrupt.

bool                ledState        = false;    // LED on/off state.
bool                oldState        =  true;    // Old LED state.
byte                pushCnt         =    0;     // Count of button pushes.
Button2             btn2;                       // Handle button connected to pin 2.

void setup() {
  Serial.begin(115200);
  Serial.printf(F("Starting Button_Test version %d.%d, Build date %s %s\n"),
                                                                VER_MAJOR,
                                                                VER_MINOR,
                                                                __DATE__,
                                                                __TIME__);
  Serial.printf(F("Arduino version %d, ButtonInterrupt version %d.%d\n"),
                                                        ARDUINO,
                                                        btn2.BUTTON_INTERRUPT_VER_MAJOR,
                                                        btn2.BUTTON_INTERRUPT_VER_MINOR);

  pinMode(LED_PIN, OUTPUT);

  btn2.start();
  Serial.printf(F("Ending setup. millis() = %lu\n"), millis());
}

void loop() {

  //if (btnPressed == true) {                                 // Button just pressed.
  if (btn2.isBtnPressed() == true) {                        // Button just pressed.
    btn2.clrBtnPressed();                                   // Clear btnPressed to be ready
                                                            //   for the next press.
    ledState = !ledState;

    if (++pushCnt >= MAX_PUSH) {
      Serial.printf(F("pushCnt %d reached. Stopping button interrupt for 5 seconds.\n"),
                                                                            pushCnt);
      pushCnt = 0;
      btn2.stop();
      btn2.breakableDelay(5000UL);
      btn2.start();
      Serial.println(F("Restarted button interrupt."));
    }
    
    #ifdef SERIAL_DEBUG
    Serial.printf(F("Button pressed. pushCnt %d, new ledState %d\n"), pushCnt, ledState);
    #endif // SERIAL_DEBUG    
  }

  #ifdef SERIAL_DEBUG
  if (oldState != ledState) {                               // ledState just changed.
    oldState = ledState;
    Serial.print("ledState ");
    Serial.println(ledState);
  }
  #endif // SERIAL_DEBUG

  if (ledState == true) {
    digitalWrite(LED_PIN,  HIGH);
    btn2.breakableDelay(1000);
    digitalWrite(LED_PIN,  LOW);
    btn2.breakableDelay(1000);
  }
  else {
    digitalWrite(LED_PIN,  HIGH);
    btn2.breakableDelay(100);
    digitalWrite(LED_PIN,  LOW);
    btn2.breakableDelay(100);
  }
}


