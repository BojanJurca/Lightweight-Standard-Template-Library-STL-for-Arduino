#include "std/console.hpp"

void setup () {
    // Serial (console) initialization
    cinit (true);                                             // two optional arguments: bool waitForSerial = false, unsigned int serialSpeed = 115200

    // Serial (console) input
    int i;
    cout << "Please enter an integer: ";
    cin >> i;

    // Serial (console) output
    cout << "you entered " << i << endl;
}

void loop () {

}
