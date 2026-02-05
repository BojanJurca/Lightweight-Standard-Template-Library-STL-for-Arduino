#include <locale.hpp>
#include <istream.hpp>
#include <ostream.hpp>


void setup () {
    cinit (); // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)

    cout << fixed << setprecision (6) << showpoint;

    cout << "Temperature of the Sun's photosphere is " << 9800 << " ℉\n";
    cout << "Stefan-Boltzmann constant is " << 5.670374419 << " · 10⁻⁸ [W·m⁻²·K⁻⁴]\n";

    #ifndef ARDUINO_ARCH_AVR
        time_t currentTime = 1761600424;
        struct tm structuredCurrentTime;
        localtime_r (&currentTime, &structuredCurrentTime);
        cout << "The time of writing this code is " << structuredCurrentTime << endl;
    #endif


    setlocale (lc_all, "en_150.UTF-8");


    cout << "Temperature of the Sun's photosphere is " <<  5430 << " ℃\n";
    cout << "Stefan-Boltzmann constant is " << 5.670374419 << " · 10⁻⁸ [W·m⁻²·K⁻⁴]\n";

    cout << "Please enter π using en_150.UTF-8 locale: ";
    double d;
    cin >> d;
    cout << d << ( abs (3.14159265359 - d) < 0.01 ? " is close enough\n" : " is not π\n" );

    #ifndef ARDUINO_ARCH_AVR
        cout << "The time of writing this code is " << structuredCurrentTime << endl;
    #endif
}

void loop () {

}