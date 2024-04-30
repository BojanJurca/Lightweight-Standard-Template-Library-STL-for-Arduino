#include "std/queue.hpp"


void setup () {
    #ifdef ARDUINO_ARCH_AVR // Assuming Arduino Mega or Uno
        Serial.begin (9600);
    #else
        Serial.begin (115200);
    #endif
    while (!Serial) 
        delay (10);
    delay (1000);


    // Inserting values into circular queue
    queue<int, 10> q;                                         // create a circular queue of integers containing max 10 elements
    for (int i = 0; i < 100; i ++)
        q.push_back (i);                                      // add elements
    for (int i = q.size () - 1; i >= 0; i--)                  // list all queue elements in FIFO order
        Serial.println (q [i]);


    // Detecting errors that occured in queue operations
    signed char e =  q.errorFlags ();
    if (!e) // OK
        Serial.println ("100 push_backs succeeded");
    else {
        // report error or check flags
        Serial.print ("insert error: ");
        switch (e) {
            case BAD_ALLOC:       Serial.println ("BAD_ALLOC"); break;
            case OUT_OF_RANGE:    Serial.println ("OUT_OF_RANGE"); break;
        }
    }


    // Try something more complicated - circular queue with running averages
    class circularQueueWithRunningAverages : public queue<unsigned long, 100> {
        public:

            unsigned long sum = 0;
            virtual void pushed_back (unsigned long& element) { sum += element; }
            virtual void popped_front (unsigned long& element) { sum -= element; }
    };

    circularQueueWithRunningAverages ra;
    for (int i = 1; i <= 1000; i++) {
        ra.push_back (i);
        Serial.println ( (float) ra.sum / (float) ra.size () ); // display running average
    }


}

void loop () {

}
