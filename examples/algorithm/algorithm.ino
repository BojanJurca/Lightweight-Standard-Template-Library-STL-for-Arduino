#include <ostream.hpp>      // cout instance for Arduino
#include <array.hpp>        // arrays for Arduino
#include <list.hpp>         // single linked lists with error handling and ability to use PSRAM for Arduino
#include <vector.hpp>       // vectors with error handling and ability to use PSRAM for Arduino
#include <algorithm.hpp>    // find, heap sort for vectors, ... merge sort for lists


void setup () {

    cinit ();                                                   // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)

    // ----- algorithm functions on C arrays -----
    cout << "----- C arrays -----\n";

    int Fibonacci [] = { 0, 1, 1, 2, 3, 5, 8, 21, 13 };
    int size = sizeof (Fibonacci) / sizeof (Fibonacci [0]);
 
    // Finding an element
    auto fca = find (Fibonacci, Fibonacci + size, 13);
    if (fca != Fibonacci + size)
        cout << "Found " << *fca << endl;    

    // Finding min, max element
    auto mca = max_element (Fibonacci, Fibonacci + size);
        cout << "Max element in the C array = " << *mca << endl;

    // Sorting
    cout << "Heap-sorted: ";
    sort (Fibonacci, Fibonacci + size);
    for (int i = 0; i < size; i++)
        cout << Fibonacci [i] << " ";
    cout << endl;


    // ----- algorithm functions on arrays -----
    cout << "----- arrays -----\n";

    array<int, 8> Lucas ( { 2, 1, 3, 4, 7, 11, 18, 29 } );

    // Finding an element
    auto flc = find (Lucas.begin (), Lucas.end (), 11);
    if (flc != Lucas.end ())
        cout << "Found " << *flc << endl;

    // Finding min, max element
    auto mlc = max_element (Lucas.begin (), Lucas.end ());
        cout << "Max element in the array = " << *mlc << endl;

    // Sorting
    cout << "Heap-sorted: ";
    sort (Lucas.begin (), Lucas.end ());
    cout << Lucas << endl;


    // ----- algorithm functions on lists -----
    cout << "----- lists -----\n";

    list<String> Grocery ( { String ("milk"), String ("bread"), String ("salad"), String ("tomatoes"), String ("eggs") } );
 
    // Finding an element
    auto fl = find (Grocery.begin (), Grocery.end (), "salad");
    if (fl != Grocery.end ())
        cout << "Found " << *fl << endl;    

    // Finding min, max element
    auto ml = max_element (Grocery.begin (), Grocery.end ());
        cout << "Max element in the list = " << *ml << endl;

    // Sorting
    cout << "Merge-sorted: ";
    sort (Grocery.begin (), Grocery.end ());
    cout << Grocery << endl;


    // ----- algorithm functions on vectors -----
    cout << "----- vectors -----\n";

    vector<char> Direction ( { 'U', 'D', 'L', 'R' } );

    // Finding an element
    auto fv = find (Direction.begin (), Direction.end (), 'R');
    if (fv != Direction.end ())
        cout << "Found " << *fv << endl;    

    // Finding min, max element
    auto mv = max_element (Direction.begin (), Direction.end ());
        cout << "Max element in the vector = " << *mv << endl;

    // Sorting
    cout << "Heap-sorted: ";
    sort (Direction.begin (), Direction.end ());
    cout << Direction << endl;
}

void loop () {

}
