#include "std/Cstring.hpp"


void setup () {
    Serial.begin (115200);
    while (!Serial)
        delay (10);
    delay (1000);


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
        Serial.print (cs2);
        char *f = stristr (cs3, "J");
        if (f)
            Serial.print (f);

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
        Serial.print ("cs3 = "); Serial.print (cs3); Serial.print (" but there was an error %i while calculating its value: "); Serial.println (cs3.errorFlags ());
        if (cs3.errorFlags () & OVERFLOW) Serial.println ("OVERFLOW");                                                // if the content should actually be longer than it fits into csString
        if (cs3.errorFlags () & OUT_OF_RANGE) Serial.println ("OUT_OF_RANGE");                                        // if substr or substring addressed non-existing position
    }


    cs3 = cs2.substr (11, 3); // please note that error information from operands is passed to the result
    Serial.print ("cs3 = "); Serial.println (cs3); 
    if (cs3.errorFlags ()) {
        Serial.print ("... but there was an error %i while calculating its value: "); Serial.println (cs3.errorFlags ()); // in spite of the error Cstring is still calculated from the data that is available
        if (cs3.errorFlags () & OVERFLOW) Serial.println ("OVERFLOW");                                                                                              // if the content should actually be longer than it fits into csString
        if (cs3.errorFlags () & OUT_OF_RANGE) Serial.println ("OUT_OF_RANGE");                                                                                      // if substr or substring addressed non-existing position
    }


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
            Serial.println ("csjson construction error? " + String (csjson.errorFlags ())); // check success
            Serial.println ("csjson length: " + String (csjson.length ())); 
            Serial.println ("csjson construction finished in " + String (millisEnd - millisStart) + " milliseconds\n");
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
            Serial.println ("Sjson construction error? " + String (error)); // check success
            Serial.println ("Sjson length: " + String (Sjson.length ())); 
            Serial.println ("Sjson construction finished in " + String (millisEnd - millisStart) + " milliseconds\n");
            // output: Sjson construction finished in 2 milliseconds
}

void loop () {

}
