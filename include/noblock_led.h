#ifndef __noblock_led__
#define __noblock_led__

#include <arduino.h>

class nb_led {
    public:
        nb_led(int led_pin) { PIN_LED = led_pin; }
        virtual ~nb_led() { ; };

        void run () {
            NowMil = millis();
            if (NowMil < StartMil) {                                //condition to check millis() reset
                StartMil = NowMil;
            }
            if ((led_state == false) && (led_value > 0 )) {         //if the led is set to be off -> turn it off bit by bit
                if (led_value <= 80) {
                    if ((NowMil-StartMil) > 3 ) { 
                        StartMil = NowMil;
                        led_value--;
                        analogWrite(PIN_LED, led_value);
                    }
                }
                else if (led_value > 80) {
                    if ((NowMil-StartMil) > 2 ) { 
                        StartMil = NowMil;
                        led_value--;
                        analogWrite(PIN_LED, led_value);
                    }
                }
            }
            if ((led_state == true) & (led_value < 255 )) {        //if the led is set to be on -> turn it on bit by bit
                if (led_value <= 80) {
                    if ((NowMil-StartMil) > 3 ) { 
                        StartMil = NowMil;
                        led_value++;
                        analogWrite(PIN_LED, led_value);
                    }
                }
                else if (led_value > 80) {
                    if ((NowMil-StartMil) > 2 ) { 
                        StartMil = NowMil;
                        led_value++;
                        analogWrite(PIN_LED, led_value);
                    }
                }
            }
        }

        void toggle ( bool STATE ) {
            led_state = STATE;
        }

    private:
        int led_value = 0;
        bool led_state = false;
        int PIN_LED;
        unsigned long StartMil = 0;
        unsigned long NowMil = 0;
};

#endif //__noblock_led__