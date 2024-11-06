# Lightweight C++ Standard Template Library (STL) for Arduino

Lightweight C++ Standard Template Library (STL) for Arduino includes some STL and std functionalities, like console, cstrings (C char arrays with C++ operators) with optional locales, lists, vectors, queues, maps and algorithms.
It runs also on AVR boards and enables error handling without needing try-catch functionality so posible run-time errors can be properly handled and the controller does not get restarted.
Internal data structures can be placed also on PSRAM if it is available, leaving heap free for other purposes.

The latest changes are about using the same syntax as C++ STL and sorting of lists.


### console


console.hpp creates two objects: cin and cout, for more standard way of communication with Serial interface, like:


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


CString.hpp defines a class template for C char arrays with C++ operators that do not overflow the internal buffer. They have an extra flag to keep and report the information about errors that occured during Cstring operations. Cstrings reside in the global memory area or on the stack, so they do not fragment the heap at all.


```C++
#include "std/Cstring.hpp"

void setup () {
    Cstring<15> cs3 = "abc";              // cs3 can hold max 15 characters and is assigned "abc" value after construction
    cs3 += "def";
    string s;                             // equivalent of Cstring<300>
}
```


### lists


Lists are single linked nodes (to preserve memory space) and have extra flag to keep and report the information about errors that occured during list operations. Lists reside either on the heap or in PSRAM (if it is available).


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


Vectors have extra flag to keep and report the information about errors that occured during vector operations. Vectors reside either on the heap or in PSRAM (if it is available).


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


Queues have extra flag to keep and report the information about errors that occured during queue operations. Queues reside either on the stack or global memory, heap or in PSRAM (if it is available).

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


Maps of key-value pairs are internally implemented as balanced binary search trees, for good performance. They have an extra flag to keep and report the information about errors that occured during map operations. Maps reside either on the heap or in PSRAM (if it is available).


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


Some algorithms functionalities are implemented, like sorting (of arrays, vectors) and finding certain or min, max elements (in arrays, lists, vectors, maps).


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


Only the framework for the locale settings is implemented. If you need your own locale settings you will have to modify the example included so that it works for you.


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
