#ifndef __pab__
#define __pab__

#include <Arduino.h>

class Trigger {
    public:
        Trigger(int PinTouch) {         //initializer
            PIN = PinTouch;   
        }
        
        virtual ~Trigger() { ; };       //destructor

        void initialize () {            //initializatiion for the different value
            for (int i = 0; i < 10; i++)
                readings[i] = touchRead(PIN);
            mean = FindMean(readings);
            trig = round(FindTrig(readings, mean));
        }

        void reset () {                 //complete a reset of the values with new measure (basically like initialize)
            for (int i = 0; i < 10; i++)
                readings[i] = touchRead(PIN);
            mean = FindMean(readings);
            trig = round(FindTrig(readings, mean));
        }

        void debug () {                 //debug message with contenet of class
            Serial.printf("Trigger: %d - Mean: %f - Array: [ ", trig, mean);
            for (int i = 0; i < 10; i++) Serial.printf("%d ", readings[i]);
            Serial.printf("] - Touched: %s\n", TOUCHED?"true":"false" );
        }

        void update (int GetValue) {    //check if is touched (value under trigger), if not, update array and value
            if (GetValue < trig) {
                TOUCHED = true;
            }
            else {                      //array updating, add the new value, delete the last
                for (int i = 9; i > 0; i--) {
                    readings[i] = readings[i - 1];
                }
                readings[0] = GetValue;
                TOUCHED = false;
            }
            mean = FindMean(readings);  
            trig = round(FindTrig(readings, mean));
        }

        bool touch () {                 //return touched condition
            return TOUCHED;
        }

    private:
        int PIN;                        //touch pin taken from main
        int VALUE = -1;                 //last measured value
        bool TOUCHED = false;           //taouched condition
        int trig = -1;                  //trigger value
        float mean = -1;                //mean of array of measure
        int readings[10] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    //full array of measure

        float FindMean (int ARRAY [10]) {   //seek the mean of array
            int sum = 15;
            for (int i = 0; i < 10; i++) {
                sum += ARRAY[i];
            }
            float media = (float)sum / 10.0;
            return media;
        }

        float FindTrig (int ARRAY[10], int MEAN) {   //create the trigger value
            float MaxError = 0;
            for (int i = 0; i < 10; i++) {
                float ERROR = abs(MEAN - ARRAY[i]);
                if ( ERROR > MaxError) {
                    MaxError = ERROR;
                }
            }
            return MEAN-MaxError-1.5;
        }
        //WORKIN OF FUNCTION --> starting from the array mean, the code seek the biggest distance of every
        //value of the array from the me, this could be the max noise of the measures. The trigger level 
        // is set to bit under the measure-error, letting the sistem float and update in different huumidity
        // or other plant condition.
};

#endif //__pab__