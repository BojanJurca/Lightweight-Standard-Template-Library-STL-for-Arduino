/*
 *  Quick start for C++ package for Arduino (ESP boards)
 *
 *  This file is part of Lightweight C++ Standard Template Library (STL) for Arduino: https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino
 * 
 *  November 26, 2024, Bojan Jurca
 */


#include "locale.hpp"
#include "Cstring.hpp"
#include "list.hpp"
#include "vector.hpp"
#include "queue.hpp"
#include "Map.hpp"
#include "algorithm.hpp"
#include "console.hpp"


/* place all the internal memory structures into PSRAM if the bord has one
    #define LIST_MEMORY_TYPE          PSRAM_MEM
    #define VECTOR_QUEUE_MEMORY_TYPE  PSRAM_MEM
    #define MAP_MEMORY_TYPE           PSRAM_MEM
    bool psramInitialized = psramInit ();
*/


void setup () {

    // Quick start with console
    cinit (true);                                             // 3 optional arguments: bool waitForSerial = false, unsigned int serialSpeed = 115200, unsigned int waitAfterSerial = 1000
    
    cout << endl << "--- console ---" << endl << endl;
    cout << "Please enter a float ... ";
    float f;
    cin >> f;
    cout << "you entered " << f << ", please note that you can use setlocale function to input/output floating point numbers in your local format" << endl;

    // cout << "Current time is " << time (NULL) << ", please note that you can use setlocale function to input/output time in your local format" << endl;


    // Quick start with Cstring (string is, by default, #defined as Cstring<300>)
    cout << endl << "--- Cstring ---" << endl << endl;

    cstring s;                                                // create a Cstring of max 25 characters on the stack
    cstring t;                                                // create a Cstring with (default) max 300 characters
    s = "thirty tree";                                        // initialize the Cstring
    s = s + ", ...";                                          // calculate with Cstring
    cout << s << endl;

    if (s.errorFlags ())                                      // check if s is in error state
        cout << "Cstring error" << endl;                      // you may want to check individual flags: err_overflow or OUT_OF_RANGE


    // Quick start with list
    cout << endl << "----- list -----" << endl << endl;

    list<String> l;                                                 // create a list containing Strings
    #ifndef ARDUINO_ARCH_AVR // assuming Arduino Mega or Uno
        l = {"banana", "apple", "orange"};                          // initialize the list from brace enclosed initializer list
    #endif
    l.push_back ("grapefruit");                                     // add element "grapefruit"
    l.push_back ("kiwi");
    l.pop_front ();                                                 // remove the first element
    l.push_front ("grapes");

    cout << "The first element = " << l.front () << endl;           // the first element in the list
    cout << "The last element = " << l.back () << endl;             // the last element in the list
    auto fl = find (l.begin (), l.end (), "grapes");                // find an element in the list
    if (fl != l.end ())
        cout << "Found " << *fl << endl;

    cout << "There are " << l.size () << " elements in the list" << endl;
    for (auto e: l)                                                 // scan all list elements
        cout << e << endl;

    // cout << l << endl;                                           // display all list elements at once

    signed char e = l.errorFlags ();                                // check if list is in error state
    if (e) {                                          
        cout << "list error, flags: " << e << endl; 
                                                                    
        if (e & err_bad_alloc)    cout << "err_bad_alloc\n";        // you may want to check individual error flags (there are only 2 possible types of error flags for lists)
        if (e & err_out_of_range) cout << "err_out_of_range\n";

        l.clearErrorFlags ();   
    }

    auto ml = max_element (l.begin (), l.end ());                   // finding min, max elements
    if (ml != l.end ())                                             // if not empty
        cout << "max element in list = " << *ml << endl;

    cout << "Sorting:\n";
    sort (l.begin (), l.end ());                                    // sorting
    for (auto e: l)
        cout << "    " << e << endl;



    // Quick start with vector
    cout << endl << "----- vector -----" << endl << endl;

    vector<int> v;                                                  // create a vector containing integers
    #ifndef ARDUINO_ARCH_AVR // assuming Arduino Mega or Uno
        v = {2, 1, 3};                                              // initialize the vector from brace enclosed initializer list
    #endif
    v.push_back (20);                                               // add element 20
    v.push_back (10);
    v.push_back (50);    
    v.push_back (40);    
    v.pop_front ();                                                 // remove the first element
    v.pop_back ();                                                  // remove the last element
    v.push_back (30);

    cout << "The element at position 2 = " << v [2] << endl;        // access an element by its position in the vector
    cout << "The first element = " << v.front () << endl;           // the first element in the vector
    cout << "The last element = " << v.back () << endl;             // the last element in the vector
    auto fv = find (v.begin (), v.end (), 20);                      // find an element in the vector
    if (fv != v.end ()) {
        cout << "Found " << *fv << " ... ";
        v.erase (fv);                                               // delete it
        cout << "and deleted" << endl;
    }

    cout << "There are " << v.size () << " elements in the vector" << endl;
    for (auto e: v)                                                 // scan all vector elements
        cout << e << endl;

    // cout << v << endl;                                           // display all vector elements at once

    /* signed char */ e = v.errorFlags ();                          // check if vector is in error state
    if (e) {                                          
        cout << "vector error, flags: " << e << endl; 
                                                                    
        if (e & err_bad_alloc)    cout << "err_bad_alloc\n";        // you may want to check individual error flags (there are only 2 possible types of error flags for vectors)
        if (e & err_out_of_range) cout << "err_out_of_range\n";

        v.clearErrorFlags ();
    }

    auto mv = min_element (v.begin (), v.end ());                    // finding min, max elements
    if (mv != v.end ())                                              // if not empty
        cout << "min element in vector = " << *mv << endl;

    cout << "Sorting:\n";
    sort (v.begin (), v.end ());                                    // sorting
    for (auto e: v)
        cout << "    " << e << endl;



    // Quick start with queue
    cout << endl << "----- queue -----" << endl << endl;

    queue<int> q;                                                   // create a queue of integers

    #ifndef ARDUINO_ARCH_AVR // assuming Arduino Mega or Uno
        q = {222, 444, 333};                                        // initialize the queue from brace enclosed initializer list
    #endif
    q.push (555);                                                   // add some elements
    q.push (888);
    q.push (999);
    q.pop ();                                                       // remove the first element

    cout << "The element at position 1 = " << q [1] << endl;        // access an element by its position in the queue
    cout << "The first element = " << q.front () << endl;           // the first element in the queue
    cout << "The last element = " << q.back () << endl;             // the last element in the queue    

    cout << "There are " << q.size () << " elements in the queue" << endl;
    for (auto e: q)                                                 // scan all queue elements
        cout << e << endl;

    // cout << q << endl;                                           // display all queue elements at once        

    cout << q << endl;                                              // display all queue elements at once

    /* signed char */ e = q.errorFlags ();                          // check if queue is in error state
    if (e) {                                          
        cout << "queue error, flags: " << e << endl; 
                                                                    
        if (e & err_bad_alloc)    cout << "err_bad_alloc\n";        // you may want to check individual error flags (there are only 2 possible types of error flags for queues)
        if (e & err_out_of_range) cout << "err_out_of_range\n";

        v.clearErrorFlags ();
    }

    auto mq = min_element (q.begin (), q.end ());                   // finding min, max elements
    if (mq != v.end ())                                             // if not empty
        cout << "min element in queue = " << *mq << endl;



    // Quick start with Map
    cout << endl << "----- Map -----" << endl << endl;

    Map<int, String> mp;                                      // create a map - key-value pairs, where key is and integer and value is string
    #ifndef ARDUINO_ARCH_AVR // assuming Arduino Mega or Uno
        mp = { {11, "eleven"}, {12, "twelve"} };              // initialize the map from brace enclosed initializer list
    #endif

    mp [1] = "one";                                           // assign value of "one" to key 1
    mp [2] = "two";
    mp [3] = "tree";
    cout << mp [1] << endl;                                   // access the value if the key is known

    mp.insert (4, "four");                                    // another way of inserting a pair
    mp.insert ( {5, "five"} );                                // another way of inserting a pair


    /* signed char */ e = mp.errorFlags ();                          // check if map is in error state
    if (e) {                                          
        cout << "map error, flags: " << e << endl; 
                                                   
        if (e & err_bad_alloc)    cout << "err_bad_alloc\n";        // you may want to check individual error flags (there are only 3 possible types of error flags for maps)
        if (e & err_out_of_range) cout << "err_out_of_range\n";
        if (e & err_not_unique) cout << "err_not_unique\n";

        mp.clearErrorFlags ();
    }

    auto findIterator = mp.find (3);
    if (findIterator != mp.end ())
        cout << "Found " << findIterator->first << " with value " << findIterator->second << endl;

    mp.erase (3);                                               // delete it
    if (mp.errorFlags () & err_not_found)
        cout << "the pair with key " << 3 << " not found" << endl;
    else
        cout << "deleted the pair with key " << 3 << endl;

    cout << "There are " << mp.size () << " pairs in the map" << endl;
    for (auto pair: mp)                                       // scan all key-value pairs in the map
        cout << pair.first << "-" << pair.second << endl;

    auto minIterator = mp.begin ();
    auto endIterator = mp.end ();
    if (minIterator != endIterator) {
        cout << "min key = " << minIterator->first << endl;
        -- endIterator;
        cout << "max key = " << endIterator->first << endl;
    }

    cout << mp << endl;                                           // display all map elements at once        


    // Quick start with algorithm (on arrays - other than what has already been demonstrated)
    cout << endl << "----- algorithm (on arrays) -----" << endl << endl;

    int arr [10] = { 3, 6, 4, 3, 3, 5, 8, 0, 9, 5 };
    int size = sizeof (arr) / sizeof (arr [0]);

    int *fa = find (arr, arr + size, 8);                            // find an element in the array
    if (fa != arr + size)
         cout << "8 found" << endl;

    cout << "Sorting:\n";
    sort (arr, arr + size);                                         // sort the array
    for (int i = 0; i < size; i ++)
        cout << "    " << arr [i] << endl;



    // Quick start with locale - uncomment the following lines and example code in locale.hpp to make this work 
    /*
    setlocale (LC_ALL, "sl_SI.UTF-8");                             
    
    cout << endl << "--- locale ---" << endl << endl;
    cout << "Please enter a float in local format ... ";
    f;
    cin >> f;
    cout << "you entered " << f << endl;
    cout << "this is how π looks like in local format " << 3.14 << endl;

    cout << "Current time in local format is " << time (NULL) << endl;

    cstring sarr [3] = {"ČDE", "ŠTU", "ABC"};
    for (int i = 0; i < 3; i++)
        sarr [i].tolower ();

    sort (sarr, sarr + 3);
    for (int i = 0; i < 3; i++)
        cout << sarr [i] << endl;
    */
}

void loop () {

}
