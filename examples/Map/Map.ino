#include "std/Map.hpp"


void setup () {
    Serial.begin (9600);
    while (!Serial) 
        delay (10);
    delay (1000);


    // Quick test
    Serial.println ("--- Quick test ---");
    Map<char, int> mp;
    mp ['a'] = 10;
    mp ['b'] = 20;
    mp ['c'] = 30;
    Serial.println (mp ['b']);
    for (auto pair: mp)
        Serial.println (String (pair->key) + "-" + String (pair->value));


    // Examples of Map constructors
    Serial.println ("--- Map constructors ---");
    Map<int, String> mp1;                                                               // empty Map where keys are integers and values are Strings
    #ifndef ARDUINO_ARCH_AVR // Assuming Arduino Mega or Uno
        Map<int, int> mp2 ( { {1, 10}, {2, 20}, {3, 30} });                             // constructor of Map <int, int> from brace enclosed initializer list
        Map<int, String> mp3 = { {4, "four"}, {3, "tree"}, {6, "six"}, {5, "five"} };   // constructor of Map <int, String> and their initialization from brace enclosed initializer list 
    #else // older bords, like Arduino Mega, ...
        Map<int, String> mp3;
        mp3.insert ( {4, "four"} );
        mp3.insert ( {3, "tree"} );
        mp3.insert ( {6, "six"} );
        mp3.insert ( {5, "five"} );
    #endif


    Map<int, String> mp4 = mp3;                                                         // copy-constructor
    // Examples of Map assignment
    Serial.println ("--- Map assignment --- ");
    #ifndef ARDUINO_ARCH_AVR // Assuming Arduino Mega or Uno
        mp1 = { {5, "five"}, {6, "six"} };
        mp3 = mp1;
    #else
        mp3.clear ();
        mp3.insert (5, "five");
        mp3.insert (6, "six");
    #endif


    // Examples of inserting a new pair
    Serial.println ("--- insert --- ");
    mp3.insert ( {7, "seven"} );
    mp3.insert ( 8, "eight" );
    mp3 [9] = "nine";


    // Examples of finding a value for a given key
    Serial.println ("--- find --- ");    
    String *pStr = mp3.find (7);
    if (pStr != NULL) // OK
        Serial.println ("Key found, its value = " + *pStr); 
    else
        Serial.println ("Key not found");

    String str = mp3 [6];
    // it is more difficult to check NOT_FOUND in this case. One possibility is to check errorFlags (), the other is to check if str is assigned the default value (zerro memory)
    if (str != "")
        Serial.println ("Key found, its value = " + str); 
    else
        Serial.println ("Key not found");


    // When the key is an object (like String) more things can go wrong and more checking is needed.
    Map<String, int> mp5;
    mp5 ["forty four"] = 44;
    mp5 ["thirty tree"] = 33;
    mp5 ["sixty six"] = 66;
    mp5 ["fifty five"] = 55;

    mp5.clearErrorFlags ();  // clear possible error flags from previous operations
    int *intValue = mp5.find ("sixty six");
    if (intValue != NULL) // OK
        Serial.println ("Key found, its value = " + String (*intValue)); 
    else {
        // the key probably doesn't exist but we can't be 100% sure that some other error didn't occur
        if (mp5.errorFlags () & NOT_FOUND)  Serial.println ("Key not found");
        else {
            Serial.print ("An error occured while searching for the key ");
            // check flags for details
            if (mp5.errorFlags () & BAD_ALLOC)      Serial.println ("BAD_ALLOC");
            if (mp5.errorFlags () & NOT_FOUND)      Serial.println ("NOT_FOUND");
            if (mp5.errorFlags () & NOT_UNIQUE)     Serial.println ("NOT_UNIQUE");
            if (mp5.errorFlags () & CANT_DO_IT_NOW) Serial.println ("CANT_DO_IT_NOW");
        }
    }


    // Example of deleting a pair for a given key
    Serial.println ("--- erase --- ");
    mp4.erase (4);


    // Examples of scanning through Map (in ascending order of keys) with and iterator
    Serial.println ("--- scan key-value pairs --- ");
    for (auto pair: mp3)
        Serial.println (String (pair->key) + "-" + pair->value);


    Serial.println ("--- first, last, min, max --- ");
    // Finding the first and the last Map (min and max keys)
    auto firstElement = first_element (mp3);
    if (firstElement) // check if first element is found (if mp3 is not empty)
        Serial.println ("first element (max key) of mp3 = (" + String ((*firstElement)->key) + "-" + (*firstElement)->value + ")");

    auto lastElement = last_element (mp3);
    if (lastElement) // check if last element is found (if mp3 is not empty)
        Serial.println ("last element (max key) of mp3 = (" + String ((*lastElement)->key) + "-" + (*lastElement)->value + ")");

    // Finding min and max Map (min and max values)
    auto minElement = min_element (mp3);
    if (minElement) // check if min element is found (if mp3 is not empty)
        Serial.println ("min element (min value) of mp3 = (" + String ((*minElement)->key) + "-" + (*minElement)->value + ")");

    auto maxElement = max_element (mp3);
    if (maxElement) // check if max element is found (if mp3 is not empty)
        Serial.println ("max element (max value) of mp3 = (" + String ((*maxElement)->key) + "-" + (*maxElement)->value + ")");


    // Detecting errors that occured in Map operations
    Serial.println ("--- error handling --- ");
    signed char e = mp3.insert ( {9, "nine"} );
    if (!e) // OK
        Serial.println ("insert succeeded");
    else {
        // report error or check flags
        Serial.print ("insert error: ");
        switch (e) {
            case BAD_ALLOC:       Serial.println ("BAD_ALLOC"); break;
            case NOT_FOUND:       Serial.println ("NOT_FOUND"); break;
            case NOT_UNIQUE:      Serial.println ("NOT_UNIQUE"); break;
            case CANT_DO_IT_NOW:  Serial.println ("CANT_DO_IT_NOW"); break;
        }
    }

    // Checking if an error has occurred only once after many Map operations
    mp3.clearErrorFlags ();  // clear possible error flags from previous operations
    for (int i = 1000; i < 1100; i++)
        mp3.insert (i, String (i));

    e = mp3.errorFlags ();
    if (!e) // OK
        Serial.println ("100 inserts succeeded");
    else {
        Serial.print ("100 inserts error: ");  // check flags for details
        if (e & BAD_ALLOC)      Serial.println ("BAD_ALLOC");
        if (e & NOT_FOUND)      Serial.println ("NOT_FOUND");
        if (e & NOT_UNIQUE)     Serial.println ("NOT_UNIQUE");
        if (e & CANT_DO_IT_NOW) Serial.println ("CANT_DO_IT_NOW");
    }


                // capacity and speed test
                Serial.println ("--- performance and capacity --- ");
                Map<unsigned long, unsigned long> mp6;
                unsigned long l;
                unsigned long startMillis = millis ();
                for (l = 1; l <= 100000; l++) {
                    if (mp6.insert ( { l, l } ))
                        break;
                }
                unsigned long endMillis = millis ();
                mp6.clear ();
                Serial.println ("Maximum number of Map<unsigned long, unsigned long> in the memory is " + String (l));
                Serial.println ("Average insert time = " + String ((float) (endMillis - startMillis) * 1000 / (float) l) + " us");
}

void loop () {

}