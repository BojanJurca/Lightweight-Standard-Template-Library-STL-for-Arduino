#include <ostream.hpp>      // cout instance for Arduino

/* place all the internal memory structures into PSRAM if it is installed on the bord
    #define VECTOR_QUEUE_MEMORY_TYPE  PSRAM_MEM
    bool psramused = psramInit ();
*/
/* define THROW_VECTOR_QUEUE_EXCEPTIONS if you want vectors and queues to throw exceptions in case of errors rather then reporting them through errorFlags () function
    #define THROW_VECTOR_QUEUE_EXCEPTIONS 
*/

// Please note that queue is inherited from vector so everithing that works for vectors works for queues as well
#include <queue.hpp>       // queues with error handling and ability to use PSRAM for Arduino


void setup () {

    cinit ();                                                 // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)

    // Create and initialize queu with some Fibonacci numbers
    queue<int> Fibonacci ( { 0, 1, 1, 2 } );

    // Perform some queue operations
    for (int i = 0; i < 10; i++) {
        cout << "popping " << Fibonacci.front ();
        Fibonacci.pop ();
        cout << ", pushing " << Fibonacci [Fibonacci.size () - 2] + Fibonacci [Fibonacci.size () - 1] << endl;
        Fibonacci.push (Fibonacci [Fibonacci.size () - 2] + Fibonacci [Fibonacci.size () - 1]);
    }

    // Check if the queue is in error state (after many operations) ...
    signed char e = Fibonacci.errorFlags ();                        
    if (e) {                                          
        cout << "queue error, flags: " << e << endl; 
         // You may want to check individual error flags (there are only 2 possible types of error flags for queues) 
        if (e & err_bad_alloc)    
            cout << "err_bad_alloc\n";
        if (e & err_out_of_range) 
            cout << "err_out_of_range\n";
        Fibonacci.clearErrorFlags (); 
    }

    // ... or check success of each individual queue operation
    if (Fibonacci.push (Fibonacci [Fibonacci.size () - 2] + Fibonacci [Fibonacci.size () - 1]))
        cout << "err_bad_alloc\n"; // the only possible error for push_back () is err_bad_alloc

    cout << "The first element = " << Fibonacci.front () << endl;   // the first element in the queue
    cout << "The last element = " << Fibonacci.back () << endl;     // the last element in the queue

    cout << "There are " << Fibonacci.size () << " elements in the queue" << endl;
    // Use index to scan the queue
    for (size_t i = 0; i < Fibonacci.size (); i++)
        cout << Fibonacci [i] << "   ";
    cout << endl;
    // Display all the queue elements at once
    cout << Fibonacci << endl;

    // Empty the queue and release its memory
    Fibonacci.clear (); // please note that AVR boards are not very good at releasing the memory
}

void loop () {

}
