// 1️⃣ define which types of data structures will use PSRAM
#define VECTOR_QUEUE_MEMORY_TYPE    PSRAM_MEM
#define LIST_MEMORY_TYPE            PSRAM_MEM
#define MAP_MEMORY_TYPE             PSRAM_MEM

// 2️⃣ initialize PSRAM
bool psramused = psramInit ();

#include <ostream.hpp>

#include <vector.hpp>
#include <queue.hpp>
#include <list.hpp>
#include <Map.hpp>


void setup () {
    cinit ();                                                 // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)

    if (!psramused)
        cout << "PSRAM not present\n";

    // 3️⃣ use queus, vectors, lists and maps created in PSARM exactly the same way as when heap memory is used
    queue<int> qFibonacci ( { 0, 1, 1, 2, 3 } );

    vector<int> vFibonacci ( { 0, 1, 1, 2, 3, 5 } );

    list<int> lFibonacci ( { 0, 1, 1, 2, 3, 5, 8 } );

    Map<const char *, const char *> Latin ( { {"deeds", "acta"},  {"not", "non"}, {"words", "verba"} } );
}

void loop () {

}
