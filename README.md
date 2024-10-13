# C++ std package for Arduino: console, strings, vectors, queues and maps (runs also on AVR boards)


Four libraries that bring some std STL functionality to Arduino. They can be used together or completely independently one from another. But why use them when there are std::vectors and std::maps, for example already available? The simple answer is: error handling. If you need to handle run-time errors by your code then these libraries may be what you are looking for.

Another benefit is the possibility of using PSRAM for internal memory structures, leaving heap memory free for other purposes.

Besides this, you may find cout and cin a convenient replacement for Serial.print...


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


### vectors


Vectors have an extra flag to keep and report the information about errors that occured during vector operations. Vectors reside either on the heap or in PSRAM (if it is available).


```C++
// #define VECTOR_MEMORY_TYPE  PSRAM_MEM
// bool psramused = psramInit ();

#include "std/vector.hpp"

vector<char> v;                           // v is vector of chars
v.push_back ('a');
v.push_back ('b');
v.push_back ('c');
Serial.println (v [1]);                   // address vector's element by it's position
for (auto e: v)                           // iterate through vector elements
    Serial.println (String (e));  
```


### queues


Circular queues with an extra flag to keep and report the information about errors that occured during queue operations. Queues reside either on the stack or global memory, heap or in PSRAM (if it is available).

```C++
// #define QUEUE_MEMORY_TYPE  PSRAM_MEM
// bool psramused = psramInit ();
#include "std/queue.hpp"

queue<int, 10> q;                         // create a circular queue of integers containing max 10 elements
for (int i = 0; i < 100; i ++)
    q.push_back (i);                      // add elements
for (int i = q.size () - 1; i >= 0; i--)  // list all queue elements in FIFO order
    Serial.println (q [i]);
```

Circular queues can be easily extended to keep running sum or running average of data entered.

```C++
template<class queueType, size_t maxSize> class queueWithRunningSum : public queue<queueType, maxSize> {
    public:

        inline queueType sum () __attribute__((always_inline)) { 
            return __sum__;
        }

        inline float avg () __attribute__((always_inline)) { 
            return (float) __sum__ / (float) queue<queueType, maxSize>::size ();
        }

        inline void push_back (queueType element) __attribute__((always_inline)) {
            __sum__ += element;
            if (queue<queueType, maxSize>::queue::size () == maxSize)
                __sum__ -= queue<queueType, maxSize>::at (0);
            queue<queueType, maxSize>::queue::push_back (element);
        }

    private:

        queueType __sum__ =  {};
};
```


### maps


Maps of key-value pairs are internally implemented as balanced binary search trees, for good performance. They have an extra flag to keep and report the information about errors that occured during map operations. Maps reside either on the heap or in PSRAM (if it is available).


```C++
// #define MAP_MEMORY_TYPE     PSRAM_MEM
// bool psramused = psramInit ()
#include "std/Map.hpp"

Map<char, int> mp;                        // mp is a map where keys are chars and values are integers
mp ['a'] = 10;
mp ['b'] = 20;
mp ['c'] = 30;
Serial.println (mp ['b']);                // address the value by it's key 
for (auto pair: mp)                       // iterate through the map key-value pairs
    Serial.println (String (pair->key) + "-" + String (pair->value));
```
