/*
 *  Quick start with C++ std package for Arduino (ESP boards)
 * 
 *  Bojan Jurca, April 27, 2024
 *
 */


#include "std/Cstring.hpp"
#include "std/vector.hpp"
#include "std/Map.hpp"
#include "std/console.hpp"


void setup () {

    // Quick start with console
    cinit (true);                                             // two optional arguments: bool waitForSerial = false, unsigned int serialSpeed = 115200

    cout << endl << "--- console ---" << endl << endl;
    cout << "Please enter an integer: ";
    int i;
    cin >> i;
    cout << "you entered " << i << endl;


    // Quick start with Cstring (string is, by default, #defined as Cstring<300>)
    cout << endl << "--- Cstring ---" << endl << endl;

    Cstring<25> s;                                            // create a Cstring of max 25 characters on the stack
    string t;                                                 // create a Cstring with (default) max 300 characters
    s = "thirtytree";                                         // initialize the Cstring
    s = s + ", ...";                                          // calculate with Cstring
    cout << s << endl;

    if (s.errorFlags ())                                      // check if s is in error state
        cout << "Cstring error" << endl;                      // you may want to check individual flags: OVERFLOW or OUT_OF_RANGE


    // Quick start with vector
    cout << endl << "--- vector ---" << endl << endl;

    vector<int> v;                                            // create a vector containing integers
    v.push_back (10);                                         // add element 10
    v.push_back (20);
    v.push_back (30);
    int f = v.find (20);                                      // find and element
    if (f >= 0)
        cout << "20 found on position " << f << endl;
    cout << "Element at position 2 is " << v [2] << endl;     // access an element by its position
    for (auto e: v)                                           // scan all vector elements
        cout << e << endl;
    cout << v << endl;                                        // output entire vector

    if (v.errorFlags ())                                      // check if v is in error state
        cout << "vector error" << endl;                       // you may want to check individual flags: BAD_ALLOC, OUT_OF_RANGE or NOT_FOUND 


    // Quick start with Map
    cout << endl << "--- Map ---" << endl << endl;

    Map<int, string> mp;                                      // create a map - key-value pairs, where key is and integer and value is string
    mp [1] = "one";                                           // assign value of "one" to key 1
    mp [2] = "two";
    mp [3] = "tree";
    cout << (mp [1]) << endl;                                 // access the value if the key is known
    for (auto pair: mp)                                       // scan all key-value pairs in the map
        cout << pair->key << "-" << pair->value << endl;
    cout << mp << endl;                                       // output entire Map

    if (mp.errorFlags ())                                     // check if mp is in error state
        cout << "Map error" << endl;                          // you may want to check individual flags: BAD_ALLOC, NOT_FOUND, NOT_UNIQUE or CANT_DO_IT_NOW
}

void loop () {

}
