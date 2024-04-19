#include "std/Map.hpp"


void setup () {
    Serial.begin (115200);
    while (!Serial) 
        delay (10);
    delay (1000);


    // Quick test
    Map<char, int> mp;
    mp ['a'] = 10;
    mp ['b'] = 20;
    mp ['c'] = 30;
    Serial.println (mp ['b']);
    for (auto pair: mp)
        Serial.println (String (pair->key) + "-" + String (pair->value));


    // Examples of Map constructors
    Map<int, String> mp1;                                                           // empty Map where keys are integers and values are Strings
    Map<int, int> mp2 ( { {1, 10}, {2, 20}, {3, 30} });                             // constructor of Map <int, int> from brace enclosed initializer list
    Map<int, String> mp3 = { {4, "four"}, {3, "tree"}, {6, "six"}, {5, "five"} };   // constructor of Map <int, String> and their initialization from brace enclosed initializer list 
    Map<int, String> mp4 = mp3;                                                     // copy-constructor


    // Examples of Map assignment
    mp1 = { {5, "five"}, {6, "six"} };
    mp3 = mp1;


    // Examples of inserting a new pair
    mp3.insert ( {7, "seven"} );
    mp3.insert ( 8, "eight" );
    
    mp3 [9] = "nine";


    // Examples of finding a value for a given key
    String *pStr = mp3.find (6);
    if (pStr != NULL) // OK
        Serial.println ("Key found, its value = " + *pStr); 
    else
        Serial.println ("Key not found");

    String str = mp3 [9];


    // When the key is an object (like String) more things can go wrong and more checking is needed.
    Map<String, int> mp5 = { {"four", 4}, {"tree", 3}, {"six", 6}, {"five", 5} }; 

    mp5.clearErrorFlags ();  // clear possible error flags from previous operations
    int *intValue = mp5.find ("six");
    if (intValue != NULL) // OK
        Serial.println ("Key found, its value = " + String (*intValue)); 
    else {
        // the key probably doesn't exist but we can't be 100% sure that some other error didn't occur
        if (mp5.errorFlags () & NOT_FOUND)  Serial.println ("Key not found");  
        else {
            Serial.print ("An error occured while searching for the key ");
            // check flags for details
            if (mp5.errorFlags () & BAD_ALLOC)      Serial.printf ("BAD_ALLOC\n");
            if (mp5.errorFlags () & NOT_FOUND)      Serial.printf ("NOT_FOUND\n");
            if (mp5.errorFlags () & NOT_UNIQUE)     Serial.printf ("NOT_UNIQUE\n");
            if (mp5.errorFlags () & CANT_DO_IT_NOW) Serial.printf ("CANT_DO_IT_NOW\n");
        }
    }


    // Example of deleting a pair for a given key
    mp4.erase (4);


    // Examples of scanning through Map (in ascending order of keys) with and iterator
    for (auto pair: mp3)
        Serial.println (String (pair->key) + "-" + pair->value);


    // Finding the first and the last Map (min and max keys)
    auto firstElement = first_element (mp3);
    if (firstElement) // check if first element is found (if mp3 is not empty)
        Serial.printf ("first element (min key) of mp3 = (%i, %s)\n", (*firstElement)->key, (*firstElement)->value.c_str ());

    auto lastElement = last_element (mp3);
    if (lastElement) // check if last element is found (if mp3 is not empty)
        Serial.printf ("last element (max key) of mp3 = (%i, %s)\n", (*lastElement)->key, (*lastElement)->value.c_str ());


    // Finding min and max Map (min and max values)
    auto minElement = min_element (mp3);
    if (minElement) // check if min element is found (if mp3 is not empty)
        Serial.printf ("min element (min value) of mp3 = (%i, %s)\n", (*minElement)->key, (*minElement)->value.c_str ());

    auto maxElement = max_element (mp3);
    if (maxElement) // check if max element is found (if mp3 is not empty)
        Serial.printf ("max element (max value) of mp3 = (%i, %s)\n", (*maxElement)->key, (*maxElement)->value.c_str ());


    // Detecting errors that occured in Map operations
    signed char e = mp3.insert ( {9, "nine"} );
    if (!e) // OK
        Serial.println ("insert succeeded");
    else {
        // report error or check flags
        Serial.printf ("insert error: ");
        switch (e) {
            case BAD_ALLOC:       Serial.printf ("BAD_ALLOC\n"); break;
            case NOT_FOUND:       Serial.printf ("NOT_FOUND\n"); break;
            case NOT_UNIQUE:      Serial.printf ("NOT_UNIQUE\n"); break;
            case CANT_DO_IT_NOW:  Serial.printf ("CANT_DO_IT_NOW\n"); break;
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
        Serial.printf ("100 inserts error: ");  // check flags for details
        if (e & BAD_ALLOC)      Serial.printf ("BAD_ALLOC\n");
        if (e & NOT_FOUND)      Serial.printf ("NOT_FOUND\n");
        if (e & NOT_UNIQUE)     Serial.printf ("NOT_UNIQUE\n");
        if (e & CANT_DO_IT_NOW) Serial.printf ("CANT_DO_IT_NOW\n");
    }


                // capacity and speed test
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
