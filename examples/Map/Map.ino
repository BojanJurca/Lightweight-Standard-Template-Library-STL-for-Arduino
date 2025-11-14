#include <ostream.hpp>      // cout instance for Arduino

/* place all the internal memory structures into PSRAM if it is installed on the bord
    #define MAP_MEMORY_TYPE           PSRAM_MEM
    bool psramused = psramInit ();
*/
/* define USE_MAP_EXCEPTIONS if you want maps to throw exceptions in case of errors rather then reporting them through errorFlags () function
    #define USE_MAP_EXCEPTIONS 
*/

#include <Map.hpp>          // maps with error handling and ability to use PSRAM for Arduino


void setup () {

    cinit ();                                                 // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)

    // Create and initialize map with some translations
    Map<const char *, const char *> Latin ( { {"deeds", "acta"},  {"not", "non"}, {"words", "verba"} } );

    // Perform some map operations, like inserting additional pairs 
    Latin.insert ( {"to", "ad"} );
    Latin.insert ( {"absurdity", "absurdum"} );
    // or use []
    Latin ["by"] = "de";
    Latin ["deed"] = "facto";


    // Check if the map is in error state (after many operations) ...
    signed char e = Latin.errorFlags ();                        
    if (e) {                                          
        cout << "map error, flags: " << e << endl; 

        // You may want to check individual error flags (there are only 3 possible types of error flags for maps)
        if (e & err_bad_alloc)    
            cout << "err_bad_alloc\n";        
        if (e & err_not_found) 
            cout << "err_not_found\n";
        if (e & err_not_unique) 
            cout << "err_not_unique\n";
        Latin.clearErrorFlags ();
    }

    // ... or check success of each individual map operation
    switch (Latin.insert ( {"law", "jure"})) {
        case err_bad_alloc: 
                            cout << "insert failed due to memory issue\n";
                            break;
        case err_not_unique:
                            cout << "the key already exists\n";
                            break;
    }

    // Use the map to (kind of) compose some Latin phrases
    cout << Latin ["deeds"] << " " << Latin ["not"] << " " << Latin ["words"] << endl;
    cout << Latin ["to"] << " " << Latin ["absurdity"] << endl;
    cout << Latin ["by"] << " " << Latin ["deed"] << endl;
    cout << Latin ["by"] << " " << Latin ["law"] << endl;

    // Finding min and max keys
    auto minIterator = Latin.begin ();
    auto endIterator = Latin.end ();
    if (minIterator != endIterator) {
        cout << "Min key = " << minIterator->first << endl;
        -- endIterator;
        cout << "Max key = " << endIterator->first << endl;
    }

    cout << "There are " << Latin.size () << " pairs in the map" << endl;
    // Use iterator to scan the map
    for (auto pair: Latin)
        cout << pair.first << "-" << pair.second << endl;
    // Display all the pairs at once
    cout << Latin << endl;

    // Empty the map and release its memory
    Latin.clear (); // please note that AVR boards are not very good at releasing the memory
}

void loop () {

}
