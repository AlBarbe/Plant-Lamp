#include <Arduino.h>
#include "pab.h"
#include "noblock_led.h"

#define PIN_INPUT     T0    //GPIO used as input signal - must chose between touch pin of the board (EX: T0, T1, etc..)
#define PIN_LED       22

Trigger trigger (PIN_INPUT);      //class meant to simplify recursive action, and clean main
nb_led led (PIN_LED);             //class to control led toggle without blocking the code

int value = 0;                    //last value measured
bool KeptTouch = false;           //bool used to track hold touch

unsigned long StartMillis1;       //first timer -- sampling speed
const int Period1 = 100;
unsigned long StartMillis2;       //second timer -- how many millis before adapting trigger level
const int Period2 = 5000;
bool LightUp = false;

void setup(){
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  trigger.initialize();
  Serial.printf("Initialized as follow:");   //debug
  trigger.debug();                           //debug
  Serial.printf("\n");                       //debug
}


void loop(){

  led.run() ;                                                 //needed to perform led dimming after toggling

  if (millis() < StartMillis1 || millis() < StartMillis2){    //protection against Millis() value reset
    StartMillis1 = 0;
    StartMillis2 = 0;
  }

  if (millis()- StartMillis1 > Period1) {                     //Timer to update touch sensor every "Period1" millisec
    value = touchRead(T0);
    trigger.update(value);
    //Serial.println(value);            //debug
    //trigger.debug();                  //debug
    StartMillis1 = millis();
  }
  
  if (trigger.touch () == true && KeptTouch == true) {        //condition to do action if kept pressed for more than "Period1"
    if (millis()- StartMillis2 > Period2) {                   //timer to update trigger reset after being touch for more than "Period2" millisec
      trigger.reset();
      StartMillis2 = millis();
      Serial.printf("Trigger reset!");  //debug
    }
  }

  if (trigger.touch () == true && KeptTouch == false) {       //condiotion to do action on press (1 time after pressed)
    StartMillis2 = millis();                                  //setting up start time for "trigger reset" timer
    KeptTouch = true;
    Serial.printf("Touched!!!");        //debug

    //===========  Put Here the list of action to do on press  ============//
    LightUp = !LightUp;
    led.toggle( LightUp );

    /*{
    if (LightUp == false) {
      for(int dutyCycle = 0; dutyCycle <= 80; dutyCycle++){   
        // changing the LED brightness with PWM
        analogWrite(PIN_LED, dutyCycle);
        delay(3);
      }
      for(int dutyCycle = 80; dutyCycle <= 255; dutyCycle++){   
        // changing the LED brightness with PWM
        analogWrite(PIN_LED, dutyCycle);
        delay(2);
      }
    }
    else if (LightUp == true) {
      for(int dutyCycle = 255; dutyCycle >= 80; dutyCycle--){
        // changing the LED brightness with PWM
        analogWrite(PIN_LED, dutyCycle);
        delay(2);
      }
      for(int dutyCycle = 80; dutyCycle >= 0; dutyCycle--){
        // changing the LED brightness with PWM
        analogWrite(PIN_LED, dutyCycle);
        delay(3);
      }
    }
    }*/
  }

  if (trigger.touch () == false && KeptTouch == true) {       //condition to do actions on release
    Serial.printf("Released!!!");       //debug
    KeptTouch = false;
  }

}
