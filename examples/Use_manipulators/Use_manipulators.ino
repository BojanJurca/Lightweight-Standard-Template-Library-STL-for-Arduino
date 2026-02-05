#include <ostream.hpp>


void setup () {
    cinit (); // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)

    cout << "\nWithout manipulators:\n";

    cout << "   Temperature of the Sun's photosphere is " <<  5430 << " ℃\n";
    cout << "   Stefan-Boltzmann constant is " << 5.670374419 << " · 10⁻⁸ [W·m⁻²·K⁻⁴]\n";


    cout << "\nWith manipulators:\n";
    
    cout << fixed << setprecision (6) << showpoint;
    cout << "   Temperature of the Sun's photosphere is " <<  5430 << " ℃\n";
    cout << "   Stefan-Boltzmann constant is " << 5.670374419 << " · 10⁻⁸ [W·m⁻²·K⁻⁴]\n";
}

void loop () {

}