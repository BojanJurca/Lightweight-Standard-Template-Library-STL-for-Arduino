#include <locale.hpp>

// Please note tht as soon as locale.hpp is included the following three locales are 
// already available: ASCII (default), en_US.UTF-8 and en_150.UTF-8.
// Any additional locale you would like to use you need to implement on your own as shown in Create_custom_locale example

#include <istream.hpp>
#include <ostream.hpp>


void setup () {
    cinit (); // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)

    cout << fixed << setprecision (6) << showpoint;

    cout << "\nUsing default locale " << getlocale ()->name () << endl;
    cout << "   Temperature of the Sun's photosphere is " << ( getlocale ()->getTemperatureUnit () == 'C' ? 5430 : 9800 ) << " °" << getlocale ()->getTemperatureUnit () << endl;
    cout << "   Stefan-Boltzmann constant is " << 5.670374419 << " · 10⁻⁸ [W·m⁻²·K⁻⁴]\n";

    #ifndef ARDUINO_ARCH_AVR
        time_t currentTime = 1761600424;
        struct tm structuredCurrentTime;
        localtime_r (&currentTime, &structuredCurrentTime);
        cout << "   The time of writing this code is " << structuredCurrentTime << endl;
    #endif


    #ifndef ARDUINO_ARCH_AVR
        setlocale (lc_all, "en_US.UTF-8");
    #else
        // AVR board do not support time_t and struct tm types so lc_time can not be set
        // ls_all includes lc_time so passing lc_all will always fail on AVR boards
        // setlocale would return false
        setlocale ((localeCategory_t) (lc_collate | lc_ctype | lc_numeric), "en_US.UTF-8");
    #endif

    cout << "\nUsing locale " << getlocale ()->name () << endl;
    cout << "   Temperature of the Sun's photosphere is " << ( getlocale ()->getTemperatureUnit () == "℃" ? 5430 : 9800 ) << " " << getlocale ()->getTemperatureUnit () << endl;
    cout << "   Stefan-Boltzmann constant is " << 5.670374419 << " · 10⁻⁸ [W·m⁻²·K⁻⁴]\n";


    #ifndef ARDUINO_ARCH_AVR
        setlocale (lc_all, "en_150.UTF-8");
    #else
        // AVR board do not support time_t and struct tm types so lc_time can not be set
        // ls_all includes lc_time so passing lc_all will always fail on AVR boards
        // setlocale would return false
        setlocale ((localeCategory_t) (lc_collate | lc_ctype | lc_numeric), "en_150.UTF-8");
    #endif

    cout << "\nUsing locale " << getlocale ()->name () << endl;
    cout << "   Temperature of the Sun's photosphere is " << ( getlocale ()->getTemperatureUnit () == "℃" ? 5430 : 9800 ) << " " << getlocale ()->getTemperatureUnit () << endl;
    cout << "   Stefan-Boltzmann constant is " << 5.670374419 << " · 10⁻⁸ [W·m⁻²·K⁻⁴]\n";

    cout << "   Please enter π using " << getlocale ()->name () << " locale: ";
    double d;
    cin >> d;
    cout << d << ( abs (3.14159265359 - d) < 0.01 ? " is close enough\n" : " is not π\n" );

    #ifndef ARDUINO_ARCH_AVR
        cout << "   The time of writing this code is " << structuredCurrentTime << endl;
    #endif
}

void loop () {

}