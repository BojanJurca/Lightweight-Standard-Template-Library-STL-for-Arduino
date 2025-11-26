#include <ostream.hpp>      // cout instance for Arduino

#include <array.hpp>        // arrays for Arduino
#include <algorithm.hpp>    // find, heap sort for vectors, ... merge sort for lists


void setup () {

    cinit ();                                                 // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)


    // Create and initialize vector with some Fibonacci numbers
    array<int, 8> Fibonacci ( { 0, 1, 1, 2, 3, 13, 5, 8 } );

    cout << "There are " << Fibonacci.size () << " elements in the array" << endl;
    // Use index to scan the array
    for (int i = 0; i < Fibonacci.size (); i++)
        cout << Fibonacci [i] << "   ";
    cout << endl;
    // Use iterator to scan the array
    for (auto f: Fibonacci)
        cout << f << "   ";
    cout << endl;
    // Display all the array elements at once
    cout << Fibonacci << endl;

    // algorith.hpp functions for arrays
    auto m = max_element (Fibonacci.begin (), Fibonacci.end ());   // finding min, max elements
    if (m != Fibonacci.end ())                                     // if not empty
        cout << "Max element in the array = " << *m << endl;

    cout << "Heap-sorted: ";
    sort (Fibonacci.begin (), Fibonacci.end ());
    cout << Fibonacci << endl;
    
}

void loop () {

}
