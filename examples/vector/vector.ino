#include <ostream.hpp>      // cout instance for Arduino

/* place all the internal memory structures into PSRAM if it is installed on the bord
    #define VECTOR_QUEUE_MEMORY_TYPE  PSRAM_MEM
    bool psramused = psramInit ();
*/
/* define THROW_VECTOR_QUEUE_EXCEPTIONS if you want vectors and queues to throw exceptions in case of errors rather then reporting them through errorFlags () function
    #define THROW_VECTOR_QUEUE_EXCEPTIONS 
*/

#include <vector.hpp>       // vectors with error handling and ability to use PSRAM for Arduino
#include <algorithm.hpp>    // find, heap sort for vectors, ... merge sort for lists


void setup () {

    cinit ();                                                 // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)


    // Create and initialize vector with some Fibonacci numbers
    vector<int> Fibonacci ( { -1, 0, 1, 1, 2, 3, 5, 8 } );

    // Reserve enough vector's memory ahead to prevent later allocations
    Fibonacci.reserve (8);

    // Perform some vector operations
    Fibonacci.pop_front ();
    Fibonacci.push_back (13);
    Fibonacci.push_back (34);

    // Check if the vector is in error state (after many operations) ...
    signed char e = Fibonacci.errorFlags ();                        
    if (e) {                                          
        cout << "vector error, flags: " << e << endl; 
         // You may want to check individual error flags (there are only 2 possible types of error flags for vectors) 
        if (e & err_bad_alloc)    
            cout << "err_bad_alloc\n";
        if (e & err_out_of_range) 
            cout << "err_out_of_range\n";
        Fibonacci.clearErrorFlags ();   
    }

    // ... or check success of each individual vector operation
    if (Fibonacci.push_back (21))
        cout << "err_bad_alloc\n"; // the only possible error for push_back () is err_bad_alloc

    cout << "The first element = " << Fibonacci.front () << endl;   // the first element in the vector
    cout << "The last element = " << Fibonacci.back () << endl;     // the last element in the vector
    auto f = find (Fibonacci.begin (), Fibonacci.end (), 13);       // find an element in the vector
    if (f != Fibonacci.end ())
        cout << "Found " << *f << endl;    

    cout << "There are " << Fibonacci.size () << " elements in the vector" << endl;
    // Use index to scan the vector
    for (size_t i = 0; i < Fibonacci.size (); i++)
        cout << Fibonacci [i] << "   ";
    cout << endl;
    // Use iterator to scan the vector
    for (auto f: Fibonacci)
        cout << f << "   ";
    cout << endl;
    // Display all the vector elements at once
    cout << Fibonacci << endl;

    // algorith.hpp functions for vectors
    auto m = max_element (Fibonacci.begin (), Fibonacci.end ());   // finding min, max elements
    if (m != Fibonacci.end ())                                     // if not empty
        cout << "Max element in the vector = " << *m << endl;

    cout << "Heap-sorted: ";
    sort (Fibonacci.begin (), Fibonacci.end ());
    cout << Fibonacci << endl;

    // Empty the vector and release its memory
    Fibonacci.clear (); // please note that AVR boards are not very good at releasing the memory
}

void loop () {

}
