#include "std/Cstring.hpp"

void setup () {
    Serial.begin (115200);
    while (!Serial) delay (10);

    // Examples of Cstring constructors
    Cstring<10> cs1;                      // cs1 can hold max 10 characters and is empty when constructed
    Cstring<15> cs2 ("abcdef");           // cs2 can hold max 15 characters and is initialized with "abc" value
    Cstring<15> cs3 = "ghijkl";           // cs3 can hold max 15 characters and is assigned "def" value after construction
    Cstring<5> csi (1234);                // csi can hold max 5 characters and is assigned "1234" value (converted from int)
    // please note that there is also available a default #define string Cstring<300>
    // there are other constructors available as well


    // Examples of Cstring usage
        // as C char array:
        Serial.println (cs2);
        Serial.printf ("%s\n", cs2);
        Serial.printf ("%s\n", &cs2 [3]);
        char *f = stristr (cs3, "J");
        if (f)
            Serial.printf ("%s\n", f);

        // as Arduino String:
        Serial.println (cs2);
        Serial.println (cs2.substring (3)); 

        // as std::string
        Serial.println (cs2);
        Serial.println (cs2.substr (3)); 


    // Examples of Cstring assignment
    cs1 = "ghi";                           // assign char array content  to s1
    cs2 = cs3;                             // assign the value of s3 to s2 (note that they are of the same type: Cstring<15>), beside content the error flags are alos copied
    cs3 = cs1;                             // assign the value of s1 to s3 in char array manner, since they are not of the same type (Cstring<10> and Cstring<15>), so error flags do not get copied!
    csi = 54321;

    // Assignment from Arduino String (in char array manner)
    String Ss3 = "ghi";
    cs3 = Ss3.c_str ();                    // assignment is performed through a pointer to char, which Arduino String exposes with c_str () member function
    // Cstring also has c_str () member function but since it is exactly the same as (char *) operator it is not really needed, so cs3 is exactly the same as cs3.c_str ()


    // Examples of using Cstring comparison operators
    if (cs2 <= cs3)
        Serial.println ("cs2 is less or equal to cs3");
    else
        Serial.println ("cs2 is greater than cs3");

    if (cs2 == cs3)
        Serial.println ("cs2 is equal to cs3");
    else
        Serial.println ("cs2 is different than cs3");
    // note that comparison can be performed even if both operands are not of the same type (Cstring<10> and Cstring<15>)
    if (cs1 <= cs2)
        Serial.println ("cs1 is less or equal to cs2");
    else
        Serial.println ("cs1 is greater than cs2");


    // Examples of using other Cstring operators
    cs1 += "jkl";
    cs1 = cs1 + "mno";
    cs2 = cs1 + 'c'; 


    // Detecting errors that occured in Cstring operations
    cs2 = "abcdefghij";

    cs3.clearErrorFlags (); // clear possible error flags from previous operations
    cs3 = cs2 + "123456789"; // please note that cs2 and cs3 are of the same type! (Cstring<15>) and the length exceeds 15 characters
    if (cs3.errorFlags ()) {
        Serial.printf ("cs3 = %s, but there was an error %i while calculating its value\n", cs3, cs3.errorFlags ());  // in spite of the error Cstring is still fully initialized up to the maximum number of characters it can contain
        if (cs3.errorFlags () & OVERFLOW) Serial.println ("OVERFLOW");                                                // if the content should actually be longer than it fits into csString
        if (cs3.errorFlags () & OUT_OF_RANGE) Serial.println ("OUT_OF_RANGE");                                        // if substr or substring addressed non-existing position
    }


    cs3 = cs2.substr (11, 3); // please note that error information from operands is passed to the result
    Serial.printf ("   cs3 = %s, cs3.errorFlags () = %i\n", cs3, cs3.errorFlags ());
    if (cs3.errorFlags ()) {
        Serial.printf ("cs3 = %s, but there was an error %i while calculating its value\n", cs3, cs3.errorFlags ());  // in spite of the error Cstring is still calculated from the data that is available
        if (cs3.errorFlags () & OVERFLOW) Serial.println ("OVERFLOW");                                                // if the content should actually be longer than it fits into csString
        if (cs3.errorFlags () & OUT_OF_RANGE) Serial.println ("OUT_OF_RANGE");                                        // if substr or substring addressed non-existing position
    }


    // memory efficiency of Cstring compared to Arduino String and std::string (demonstration for this particular case only, efficiency depends on the lenght of the text):

            // Cstring uses only global or stack memory (unless you explicitly put it on the heap), so all the memory used will be sizeof (ftest)
            Cstring<30> ftest = "This is not bad at all.";
            Serial.printf ("Cstring<30> ftest uses %i bytes of stack memory (always N + 3), storage efficiency = %f\n", sizeof (ftest), (float) ftest.length () / (float) sizeof (ftest));
            // output: Cstring<30> ftest uses 33 bytes of stack memory (always N + 3), storage efficiency = 0.696970

            // Arduino String uses global or stack memory, but also the heap
            size_t heapStart = ESP.getFreeHeap ();
            String Stest = "This is not bad at all.";
            size_t heapUsed = heapStart - ESP.getFreeHeap ();
            Serial.printf ("String Stest uses %i bytes of stack memory (always 16) but also %i bytes of heap, altogether %i bytes, storage efficiency = %f\n", sizeof (Stest), heapUsed, sizeof (Stest) + heapUsed, (float) Stest.length () / (float) (sizeof (Stest) + heapUsed));
            // output: String Stest uses 16 bytes of stack memory (always 16) but also 48 bytes of heap, altogether 64 bytes, storage efficiency = 0.359375


    // time efficiency of Cstring compared to Arduino String and std::string (demonstration for this particular case only):

            // construct JSON array of numbers 1 .. 100 in Cstring with error checking
            unsigned long millisStart = millis ();
            Cstring<500> csjson;

                csjson = "'[\"";
                for (int i = 1; i <= 99; i++) {
                    csjson += i;
                    csjson += "\",\"";
                }
                csjson += "100\"]'";

            unsigned long millisEnd = millis ();
            Serial.printf ("csjson construction error? %i\n", csjson.errorFlags ()); // check success
            Serial.printf ("csjson length: %i : %s\n", csjson.length (), csjson); 
            Serial.printf ("csjson construction finished in %lu milliseconds\n", millisEnd - millisStart);
            // output: csjson construction finished in 11 milliseconds
            //         this is more than Arduino Strings or std::strings need, for C char arrays do not keep string length information and hence JSON construction requires O(n^2) steps

            // construct JSON array of numbers 1 .. 100 in Arduino String with error checking
            millisStart = millis ();
            String Sjson;
            bool error = false;

                if (!Sjson.concat ("'[\"")) error = true;
                for (int i = 1; i <= 99; i++) {
                    String tmp (i);
                    if (!tmp) error = true;
                    if (!Sjson.concat (tmp)) error = true;
                    if (!Sjson.concat ("\",\"")) error = true;
                }
                if (!Sjson.concat ("100\"]'")) error = true;

            millisEnd = millis ();
            Serial.printf ("Sjson construction error? %i\n", error); // check success
            Serial.printf ("Sjson length: %i : %s\n", Sjson.length (), Sjson.c_str ()); 
            Serial.printf ("Sjson construction finished in %lu milliseconds\n", millisEnd - millisStart);
            // output: Sjson construction finished in 2 milliseconds

}

void loop () {

}
