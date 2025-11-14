#include <locale.hpp>
// #include <istream.hpp>
#include <ostream.hpp>


/*

    Create your own locale. The following is an example for sl_SI.UTF-8.

    The Slovenian alphabet is based on the Latin script, enriched with three additional letters: Č, Š, and Ž. In practice, 
    texts may also include standard English letters and characters from other former Yugoslav republics, notably Ć and Đ, 
    which are commonly encountered in names.

    To ensure proper collation and character support, the full set of letters we aim to handle includes:

               --------- 2 byte unicode characters -----------------
              | |   |                               |               |
        A B C Č Ć D Đ E F G H I J K L M N O P Q R S Š T U V W X Y Z Ž
              | |   |                               |               |
        a b c č ć d đ e f g h i j k l m n o p q r s š t u v w x y z ž
    
*/


// 1️⃣ Derive sl_SI_UTF_8_locale class from locale base class (give it a name according to your locale)
class sl_SI_UTF_8_locale : public locale { 
    public:
        // 2️⃣ override locale name
        const char* name () const override { return "sl_SI.UTF-8"; } 

        // 3️⃣ override strcoll function that lc_collate would use with your own
        int charorder (const utf8char& u8c) {
            // Expand spacing between ASCII codes by multiplying each code by 3,
            // allowing room to insert up to two additional characters between any pair (e.g., between 'C' and 'D').
            if ((unsigned char) *u8c.c_str < 0x80) // 1 byte UTF-8 character = ASCII character
                return  3 * *u8c.c_str;
            else // multi-byte UTF-8 character
                     if (u8c == utf8char ("Č")) return 3 * 'C' + 1;
                else if (u8c == utf8char ("č")) return 3 * 'c' + 1;
                else if (u8c == utf8char ("Ć")) return 3 * 'C' + 2;
                else if (u8c == utf8char ("ć")) return 3 * 'c' + 2;
                else if (u8c == utf8char ("Đ")) return 3 * 'D' + 1;
                else if (u8c == utf8char ("đ")) return 3 * 'd' + 1;
                else if (u8c == utf8char ("Š")) return 3 * 'S' + 1;
                else if (u8c == utf8char ("š")) return 3 * 's' + 1;
                else if (u8c == utf8char ("Ž")) return 3 * 'Z' + 1;
                else if (u8c == utf8char ("ž")) return 3 * 'z' + 1;
            return 3 * *u8c.c_str; // unhandeled UTF-8 character
        }

        int strcoll (const char *s1, const char *s2) {
            utf8_iterator it1 (s1); // 4️⃣ use utf8_iterators to iterate through strings of characters
            utf8_iterator end1 = utf8_iterator (s1 + strlen (s1));
            utf8_iterator it2 (s2);
            utf8_iterator end2 = utf8_iterator (s2 + strlen (s2));

            while (it1 <= end1 && it2 <= end2) { // please note that this includes the ending 0
                if (charorder (*it1) < charorder (*it2))
                    return -1; // s1 < s2
                else if (charorder (*it1) > charorder (*it2))
                    return 1; // s1 > s2
                ++it1;
                ++it2;
            }

            return 0; // s1 == s2
        }

        // 5️⃣ override toupper and tolower functions that lc_ctype would use with your own
        bool toupper (char* ps) {
            utf8_iterator end = utf8_iterator (ps + strlen (ps));
            for (utf8_iterator it (ps) /* begin */; it < end; ++it) { // 6️⃣ use utf8_iterator to iterate through string of characters

                if ((unsigned char) *(it.get ()) < 0x80) { // 1 byte UTF-8 character = ASCII character
                    if (*(it.get ()) >= 'a' && *(it.get ()) <= 'z')
                        *(it.get ()) = (*(it.get ()) - ('a' - 'A'));
                } else { // multi-byte UTF-8 character
                    // please note that the substitution like this works only when upper case letters occupy the same number of bytes as lower case letters
                    if (*it == utf8char ("č"))
                        it.set (utf8char ("Č"));
                    else if (*it == utf8char ("ć"))
                        it.set (utf8char ("Ć"));
                    else if (*it == utf8char ("đ"))
                        it.set (utf8char ("Đ"));
                    else if (*it == utf8char ("š"))
                        it.set (utf8char ("Š"));
                    else if (*it == utf8char ("ž"))
                        it.set (utf8char ("Ž"));
                }

            }
            return true;
        }

        bool tolower (char* ps) {
            utf8_iterator end = utf8_iterator (ps + strlen (ps));
            for (utf8_iterator it (ps) /* begin */; it < end; ++it) {

                if ((unsigned char) *(it.get ()) < 0x80) { // 1 byte UTF-8 character = ASCII character
                    if (*(it.get ()) >= 'A' && *(it.get ()) <= 'Z')
                        *(it.get ()) = (*(it.get ()) + ('a' - 'A'));
                } else { // multi-byte UTF-8 character
                    // please note that the substitution like this works only when lower case letters occupy the same number of bytes as upper case letters
                    if (*it == utf8char ("Č"))
                        it.set (utf8char ("č"));
                    else if (*it == utf8char ("Ć"))
                        it.set (utf8char ("ć"));
                    else if (*it == utf8char ("Đ"))
                        it.set (utf8char ("đ"));
                    else if (*it == utf8char ("Š"))
                        it.set (utf8char ("š"));
                    else if (*it == utf8char ("Ž"))
                        it.set (utf8char ("ž"));
                }

            }
            return true;
        }

        // 7️⃣ override getDecimalSeparator and getThousandsSeparator functions that lc_numeric would use with your own
        char getDecimalSeparator () const override { return ','; }
        char getThousandsSeparator () const override { return '.'; }

        // 8️⃣ override getTimeFormat function that lc_time would use with your own
        const char* getTimeFormat () const override { return "%d.%m.%Y %H:%M:%S"; }
};
// 9️⃣ Create a working instance of your class and insert it into supported locale list
bool __sl_SI_UTF_8_locale__ = addlocale (new sl_SI_UTF_8_locale);


void setup () {
    cinit (); // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)

    setlocale (lc_all, "sl_SI.UTF-8"); // 🔟 call setlocale to choose the locale you just have created

    // lc_ctype

    String scientist = "Jožef Stefan";
    toupper (scientist);
    cout << scientist << " was a Carinthian Slovene physicist, mathematician, and poet of the Austrian Empire.\n";


    // lc_collate

    String athlet [] = { "Luka Dončić", "Tadej Pogačar", "Anže Kopitar", "Primož Rogljič" };
    // Sort
    int n = sizeof (athlet) / sizeof (athlet [0]);
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i; j < n; j++) {
            if (strcoll (athlet [i], athlet [j]) > 0) {
                String tmp = athlet [i];
                athlet [i] = athlet [j];
                athlet [j] = tmp;
            }
        }
    }
    cout << "\nSorted athlets:\n";
    for (int i = 0; i < n; i++)
        cout << "   " << athlet [i] << endl;


    // lc_time

    #ifndef ARDUINO_ARCH_AVR
        setenv ("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
        tzset ();
        time_t currentTime = 1761600424;
        struct tm structuredCurrentTime;
        localtime_r (&currentTime, &structuredCurrentTime);
        cout << "\nThe time of writing this code is " << structuredCurrentTime << endl;
    #endif
}

void loop () {

}
