/*******************************************************************************************
 *
 * The ButtonInterrupt class debounces a buttun using an interrupt.
 * The button should be connected between GND and a pin that supports interrupts.
 * start() should be run at the end of setup() to start handling button interrupts.
 * isBtnPressed() returns true when the button is pressed.
 * clrBtnPressed() should be run when you handle the button.
 * stop() should be run to quit handling button interrupts.
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
 
template <byte N> class  ButtonInterrupt
{
  public:
   static const byte   BUTTON_INTERRUPT_VER_MAJOR     =     1; // Major version.
   static const byte   BUTTON_INTERRUPT_VER_MINOR     =     1; // Minor version.
   static const byte   DEFAULT_DEBOUNCE               =    50; // Default debounce time.
    
    ButtonInterrupt(void) {}

    static bool  isBtnPressed(void) { return _btnPressed; }

    static void clrBtnPressed(void) { _btnPressed = false; }

    static start(byte debounceTime = DEFAULT_DEBOUNCE) {
      
      //DEBUG(F("Entering ButtonInterrupt::start, debounceTime"), debounceTime, F("Pin"), N);
      _btnPressed   = false;
      _endTime = millis() + _END_TIME;  // Ignore interrupt that may occur
      _debounceTime = debounceTime;     // immediately after ISR attached.
      pinMode(N, INPUT_PULLUP);
      _btnState = digitalRead(N);
      attachInterrupt(digitalPinToInterrupt(N), _btnISR, CHANGE);
    }

    static stop(void) {
      detachInterrupt(digitalPinToInterrupt(N));
      //DEBUG(F("Entering ButtonInterrupt::stop"));
      _btnPressed = false;
    }
    
    static bool breakableDelay(unsigned long delay) {
      unsigned long             _endTime = millis() + delay;  // End time in ms.

      while (millis() < _endTime) {
        if (_btnPressed == true) {                            // Return immediately with false
          return false;                                       //   if button pressed.
        }
      }
  
      return true;                                            // Return true if delay finished.
    }
    
  protected:
    static const byte     _END_TIME = 100UL;
    static volatile bool  _btnPressed;
    static byte           _btnState;
    static unsigned long  _endTime;
    static byte           _debounceTime;

    static void _btnISR(void) {
      unsigned long         currentTime =  millis();          // Current time in ms.            

      //DEBUG(F("Entering ButtonInterrupt::btnISR, free RAM "), FREE_RAM);
      if (_endTime < currentTime) {                           // Ignore interrupts caused
                                                              //   by switch bounce.

        _endTime = currentTime + _debounceTime;               // Set lastInterrupt to be ready
                                                              //   for next press or release.

        _btnState = !_btnState;                               // Toggle bntState on first
                                                              //   button down or up interrupt.
 
        if (_btnState == LOW) {                               // Set btnState if button
          _btnPressed = true;                                 //   just pressed.
        }
      }
    }
};

/*
 * Static initializers. These are required or you get a linker error.
 */
template<byte N> volatile bool  ButtonInterrupt<N>::_btnPressed    = false;
template<byte N> byte           ButtonInterrupt<N>::_btnState      = HIGH;
template<byte N> unsigned long  ButtonInterrupt<N>::_endTime       = 0UL;
template<byte N> byte           ButtonInterrupt<N>::_debounceTime  = 0;

typedef ButtonInterrupt<2> Button2;
typedef ButtonInterrupt<3> Button3;
