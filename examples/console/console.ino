#include "std/console.hpp"


void setup () {
    // Serial (console) initialization
    cinit (true);                                             // 3 optional arguments: bool waitForSerial = false, unsigned int serialSpeed = 115200, unsigned int waitAfterSerial = 1000

    // Serial (console) input
    int i;
    cout << "Please enter an integer: ";
    cin >> i;

    // Serial (console) output
    cout << "you entered " << i << endl;

    // Try something more complicated
    String s = " cmbination";
    char c [] = " of";
    cout << "Let's try" << s << c << " different text data types\r\n";
}

void loop () {

}
