// #include <locale.hpp>       // locale for Arduino
// #include <istream.hpp>      // cin instance for Arduino
#include <ostream.hpp>      // cout instance for Arduino
// #include <Cstring.hpp>      // C strings that use stack memory, with C++ operators, UTF-8 awareness and error handling for Arduino
// #include <list.hpp>         // single linked lists with error handling and ability to use PSRAM for Arduino
#include <vector.hpp>       // vectors with error handling and ability to use PSRAM for Arduino
// #include <queue.hpp>        // queues with error handling and ability to use PSRAM for Arduino
#include <Map.hpp>          // maps with error handling and ability to use PSRAM for Arduino
// #include <algorithm.hpp>    // find, heap sort for vectors, ... merge sort for lists
// #include <complex.hpp>      // complex numbers for Arduino


void setup () {

    cinit (true);                                               // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)


    // Initialize vector with some Fibonacci numbers
    vector<int> Fibonacci ( { -1, 0, 1, 1, 2, 3, 5 } );

    // Perform some vector operations
    Fibonacci.pop_front ();
    Fibonacci.push_back (13);
    Fibonacci.push_back (21);

    // Check possible errors
    signed char e = Fibonacci.errorFlags ();                    // check if vector F is in error state
    if (e) {                                          
        cout << "Fibonacci error, flags: " << e << endl;                                      
        // you may want to check individual error flags (there are only 2 possible types of error flags for vectors)
        if (e & err_bad_alloc)    
            cout << "err_bad_alloc\n";         
        if (e & err_out_of_range) 
            cout << "err_out_of_range\n";

        Fibonacci.clearErrorFlags ();
    }

    // Output vector elements using index 
    for (size_t i = 0; i < Fibonacci.size (); i++)
        cout << Fibonacci [i] << " ";
    cout << endl;

    // Output vector elements using iterator
    for (auto f: Fibonacci)
        cout << f << " ";
    cout << endl;

    // Empty the vector and release its memory
    Fibonacci.clear (); // AVR boards are not very good at releasing the memory

    // Initialize map with some Morse codes
    Map<char, String> Morse ({ {'A', "•–"}, {'B', "–•••"}, {'R', "•–•"}, {'D', "–••"}, {'U', "••–"}, {'I', "••"}, {'N', "–•"} });

    // Perform some map operations
    Morse ['O'] = "–––";
    Morse.erase ('B');

    // Use the map to translate latin script to Morse script
    char latin [] = "ARDUINO";
    for (int i = 0; latin [i]; i++)
        cout << Morse [latin [i]] << "   ";
    cout << endl;

    // Check possible errors
    /* signed char */ e = Morse.errorFlags ();                  // check if map is in error state
    if (e) {                                          
        cout << "Morse error, flags: " << e << endl; 
        // You may want to check individual error flags (there are only 3 possible types of error flags for maps)
        if (e & err_bad_alloc)
            cout << "err_bad_alloc\n";        
        if (e & err_out_of_range) 
            cout << "err_out_of_range\n";
        if (e & err_not_unique) 
            cout << "err_not_unique\n";

        Morse.clearErrorFlags ();
    }

    // Empty the map and release its memory
    Morse.clear (); // please note that AVR boards are not very good at releasing the memory
}

void loop () {

}
