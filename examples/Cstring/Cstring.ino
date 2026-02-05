#include <locale.hpp>       // locale for Arduino
#include <ostream.hpp>      // cout instance for Arduino
#include <Cstring.hpp>      // C strings that use stack memory, with C++ operators, UTF-8 awareness and error handling for Arduino


void setup () {

    cinit ();                                                 // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)


    // define string with max 30 characters (ending 0 is not included)
    Cstring<30> s = "Hello world!";

    // use it as C string ...
    cout << s << endl;
    int l = strlen (s);
    for (int i = 0; i < l ; i++)
        cout << s [i] << " ";
    cout << endl; 

    // ... or C++ string
    s += " Hello to you too!!!";
    cout << s << endl;

    // Check if the string is in error state (after many operations)
    signed char e = s.errorFlags ();                        
    if (e) {                                          
        cout << "Cstring error, flags: " << e << endl; 
         // You may want to check individual error flags (there are only 2 possible types of error flags for vectors) 
        if (e & err_overflow)    
            cout << "overflow\n";
        if (e & err_out_of_range)
            cout << "err_out_of_range\n";
        s.clearErrorFlags ();   
    }

    // cstring is by deault defined as Cstring<300>
    cstring t;

    // locale awareness
    setlocale (lc_all, "en_150.UTF-8");
    s = "π = ";
    s += 3.141592653589793;
    cout << s << endl;

    // UTF-8 awareness
    s = "Hello 🌎!";
    cout << "length = " << s.length () << endl;
    cout << "characters = " << s.characters () << endl;

    // C++ functions
    size_t f = s.find ("!");
    if (f != npos)
        cout << "Found ! at position " << f << endl;

    // Arduino functions
    int i = s.indexOf ("ll");
    if (i >= 0)
        cout << "Found ll at position " << i << endl;
}

void loop () {

}
