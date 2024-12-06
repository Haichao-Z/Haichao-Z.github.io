#include "Keyboard.h"

void setup() {
 pinMode(LED_BUILTIN, OUTPUT);
 
 DDRC &= ~(1<<PC6);
 DDRE &= ~(1<<PE6);
 DDRB &= ~(1<<PB5);
 DDRB &= ~(1<<PB6);
 
 Keyboard.begin();
 delay(2000);
}

void loop() {
   if(PINC & (1<<PC6)) {  // Switch Screen
       digitalWrite(LED_BUILTIN, HIGH);
       
       Keyboard.press(KEY_LEFT_ALT);
       Keyboard.press(KEY_TAB);
       delay(100);
       Keyboard.releaseAll();
       
       digitalWrite(LED_BUILTIN, LOW);
       delay(5000);
   }
   else if(PINE & (1<<PE6)){  // Open Google Chrome
      digitalWrite(LED_BUILTIN, HIGH);
      
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('r');
      
      delay(100);
      Keyboard.releaseAll();
      delay(500);
      
      Keyboard.print("chrome");
      delay(100);
      Keyboard.press(KEY_RETURN);
      delay(200);
      Keyboard.releaseAll();
      
      digitalWrite(LED_BUILTIN, LOW);

      delay(10000);  
      
   }
   else if(PINB & (1<<PB5)){  // Screen Shot
      digitalWrite(LED_BUILTIN, HIGH);
      
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('s');
      delay(100);
      Keyboard.releaseAll();
      delay(500);
      
      Keyboard.print("google Chrome");
      delay(100);
      Keyboard.press(KEY_RETURN);
      delay(100);
      Keyboard.release(KEY_RETURN);
      
      digitalWrite(LED_BUILTIN, LOW);
      delay(5000);  
   }
   else if(PINB & (1<<PB6)){ // Task Manager
      digitalWrite(LED_BUILTIN, HIGH);
      
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_ESC);
      delay(100);
      Keyboard.releaseAll();
      
      digitalWrite(LED_BUILTIN, LOW);
      delay(5000);  
   }
   else {
     digitalWrite(LED_BUILTIN, LOW);
   }

   Keyboard.releaseAll();
       
}