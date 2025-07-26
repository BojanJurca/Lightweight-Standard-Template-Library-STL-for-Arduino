# Lightweight C++ Standard Template Library (STL) for Arduino

The Lightweight C++ STL for Arduino provides selected features from the standard C++ library, including console, cstrings (C-style character arrays with C++ operators) with optional locale support, as well as containers like lists, vectors, queues, maps, basic algorithms and complex numbers.


## Why use this library instead of the standard STL?


Here are a few reasons you might prefer this lightweight version:

- You need to reliably detect and handle run-time errors. Standard STL may simply reset the controller when an error occurs.
- You want to detect and manage errors in string operations, which Arduino String often ignores.
- You're using an AVR board (basic data types are supported even on AVR).
- You prefer the convenience of using standard C++ cin and cout over Serial.print.
- You need UTF-8 locale-aware string operations.
- You want to leverage PSRAM if your board has it built in.


## The latest changes


**August 12, 2025**: 
   - support for ostream manipulators showpoint, noshowpoint, fixed, defaultfloat, setprecision,
   - locale en_150.UTF-8 is already supported, other locales need some programming 


### console


The console module introduces the familiar C++ cin and cout objects, enabling a more idiomatic and streamlined interface for serial communication. This approach mirrors conventional C++ programming practices, making input and output operations on Arduino more intuitive for experienced C++ developers.


```C++
#include "std/console.hpp"

void setup () {
    // Serial (console) initialization
    cinit (true);                         // two optional arguments: bool waitForSerial = false, unsigned int serialSpeed = 115200

    // Serial (console) input
    int i;
    cout << "Please enter an integer ... ";
    cin >> i;

    // Serial (console) output
    cout << "you entered " << i << endl;
}
```


### strings


The CString class template provides safe, C-style character arrays augmented with C++ operators. Unlike standard char [], these strings offer built-in error detection and never overflow their internal buffer. They are stored either on the stack or in the global memory segment, which prevents heap fragmentation.


```C++
#include "std/Cstring.hpp"

void setup () {
    Cstring<15> cs3 = "abc";              // cs3 can hold max 15 characters and is assigned "abc" value after construction
    cs3 += "def";
    cstring s;                            // equivalent of Cstring<300>
}
```


### lists


This implementation provides singly linked lists designed with memory efficiency in mind. Each list maintains an internal flag that tracks and reports errors during list operations. Lists can be allocated on the heap or in PSRAM when available.


```C++
// #define LIST_MEMORY_TYPE  PSRAM_MEM
// bool psramused = psramInit ();

#include "std/list.hpp"

cinit (true);

list<String> l;                           // l is list of Strings
l.push_back ("grapefruit");
l.push_back ("kiwi");
l.push_front ("grapes");
for (auto e: l)                           // iterate through list elements
    cout << e << endl;
```


### vectors


Vectors offer dynamic arrays with automatic memory management and enhanced safety features. Like other containers in this library, vectors maintain internal flags to detect and report operation-related errors. They are stored either on the heap or in PSRAM if it exists on the hardware.


```C++
// #define VECTOR_QUEUE_MEMORY_TYPE  PSRAM_MEM
// bool psramused = psramInit ();

#include "std/vector.hpp"

cinit (true);

vector<char> v;                           // v is vector of chars
v.push_back ('a');
v.push_back ('b');
v.push_back ('c');
Serial.println (v [1]);                   // address vector's element by it's position
for (auto e: v)                           // iterate through vector elements
    Serial.println (String (e));  
```


### queues


Queues in this library offer first-in, first-out data structures that can be located in global memory, the stack, heap, or PSRAM. They include robust error handling flags for runtime diagnostics, making them safer for embedded environments.


```C++
// #define VECTOR_QUEUE_MEMORY_TYPE  PSRAM_MEM
// bool psramused = psramInit ();
#include "std/queue.hpp"

cinit (true);

queue<int> q;                             // q is queue of integers
q.push (555);
q.push (888);
q.push (999);
q.pop ();                                                       
for (auto e: q)
    cout << e << endl;
```


### maps


Key-value pairs are organized using internally balanced binary search trees to ensure efficient lookup and insertion. Maps report operation errors via dedicated flags and may reside on the heap or in PSRAM. This design provides solid performance while ensuring runtime integrity.


```C++
// #define MAP_MEMORY_TYPE     PSRAM_MEM
// bool psramused = psramInit ()
#include "std/Map.hpp"

cinit (true);

Map<char, int> mp;                        // mp is a map where keys are chars and values are integers
mp ['a'] = 10;
mp ['b'] = 20;
mp ['c'] = 30;
cout << mp ['b'] << endl;                 // address the value by it's key 
for (auto pair: mp)                       // scan all key-value pairs in the map
    cout << pair.first << "-" << pair.second << endl; // first = key, second = value
```


### algorithms


Selected algorithmic functions are included, such as sorting arrays and containers, finding minimum or maximum elements, and searching. These utilities are adapted for the constraints of microcontrollers, making them practical without requiring the full STL overhead.


```C++
#include "std/algorithm.hpp"

cinit (true);

int arr [10] = { 3, 6, 4, 3, 3, 5, 8, 0, 9, 5 };
int size = sizeof (arr) / sizeof (arr [0]);

int *fa = find (arr, arr + size, 8);      // find an element in the array
if (fa != arr + size)
        cout << "8 found" << endl;

sort (arr, arr + size);                   // sort the array
for (int i = 0; i < size; i ++)
    cout << arr [i] << endl;
```


### locale


Locale support is implemented as a framework with foundational components that can be extended for localization requirements, such as UTF-8 character processing or localized number formatting. Custom locales can be built by modifying the provided examples and configuration.


```C++
#include "std/locale.hpp"
#include "std/Cstring.hpp"
#include "std/algorithm.hpp"
#include "std/console.hpp"

cinit (true);

setlocale (LC_ALL, "sl_SI.UTF-8");                             

cout << "Please enter a float in local format ... ";
f;
cin >> f;
cout << "you entered " << f << endl;

cout << "Current time in local format is " << time (NULL) << endl;

cstring sarr [3] = {"ČDE", "ŠTU", "ABC"};
for (int i = 0; i < 3; i++)
    sarr [i].tolower ();

sort (sarr, sarr + 3);
for (int i = 0; i < 3; i++)
    cout << sarr [i] << endl;
```


### complex numbers


A C++ standards-compliant template for complex numbers is included, useful for applications such as digital signal processing (e.g., FFT operations). This module integrates seamlessly with other mathematical functions available in the library.


```C++
#include "std/console.hpp"
#include "std/complex.hpp"

cinit (true);

// FFT example
cout << "\nFFT - frequency example\n";
complex<float> signal [16];
for (int i = 0; i < 16; i++)
    signal [i] = { sin (i) + cos (2 * i), 0 };
complex<float> frequency [16];
fft (frequency, signal);
for (int i = 0; i < 16 / 2; i++)
    cout << "magnitude [" << i << "] = " << abs (frequency [i]) << endl;
```


## Things to consider when using AVR boards

Memory management on AVR boards is limited and not suitable for frequent dynamic allocations and deletions. This limitation may lead to memory leaks, especially with complex types such as String. For best results, avoid repeated insertion and deletion in dynamic containers on AVR systems.

