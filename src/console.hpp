/*
 *  console.hpp for Arduino
 * 
 *  This file is part of Lightweight C++ Standard Template Library (STL) for Arduino: https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino
 * 
 *  Aug 12, 2025, Bojan Jurca
 *  
 */


#ifndef __CONSOLE_HPP__
    #define __CONSOLE_HPP__


    // ----- TUNNING PARAMETERS -----

    #define __CONSOLE_BUFFER_SIZE__ 64 // max 63 characters in internal buffer


    // ----- CODE -----


    // Serial initialization
    #ifdef ARDUINO_ARCH_AVR // Assuming Arduino Mega or Uno
        void cinit (bool waitForSerial = false, unsigned int serialSpeed = 9600, unsigned int waitAfterSerial = 1000) {
            Serial.begin (serialSpeed);
            if (waitForSerial)
                while (!Serial) 
                    delay (10);
            delay (waitAfterSerial);
        }
    #else
        void cinit (bool waitForSerial = false, unsigned int serialSpeed = 115200, unsigned int waitAfterSerial = 1000) {
            Serial.begin (serialSpeed);
            if (waitForSerial)
                while (!Serial) 
                    delay (10);
            delay (waitAfterSerial);
        }
    #endif


    // ostream
    #define endl "\r\n"

    enum ostreamManipulator {
        precision0 = 0,
        precision1 = 1,
        precision2 = 2,
        precision3 = 3,
        precision4 = 4,
        precision5 = 5,
        precision6 = 6,
        precision7 = 7,
        precision8 = 8,
        precision9 = 9,
        precision10 = 10,
        precision11 = 11,
        precision12 = 12,
        precision13 = 13,
        precision14 = 14,
        precision15 = 15,
        precision16 = 16,
        precision17 = 17,
        precision18 = 18,
        precision19 = 19,
        showpoint,
        noshowpoint,
        defaultfloat,
        // scientific, not (yet) supported
        fixed
    };

    #define setprecision(x) ((ostreamManipulator) x)

    class ostream {

        private:

            bool __showpoint__ = false;                     // not set by default
            ostreamManipulator __fpOutput__ = defaultfloat; // by default
            int __precision__ = 6;                          // default precision
            char __floatFormat__ [6] = "%.6f";              // for default precision
            char __doubleFormat__ [7] = "%.6lf";            // for default precision

        public:

            ostream& operator << (const char* value) {
                Serial.print (value);
                return *this;
            }

            template<typename T>
            ostream& operator << (const T& value) {
                Serial.print (value);            
                return *this;
            }

    };


    // explicit ostream class specializations for const char *

/*
    template<>
    ostream& ostream::operator << <const char *> (const char *value) {
        Serial.print (value);
    }
*/

    // explicit ostream class specializations for manipulators

    template<>
    ostream& ostream::operator << <ostreamManipulator> (const enum ostreamManipulator& manipulator) {
        switch (manipulator) {
            case showpoint:
                                __showpoint__ = true;
                                break;
            case noshowpoint:
                                __showpoint__ = false;
                                break;
            case fixed:         
            case defaultfloat:  
                                __fpOutput__ = manipulator;
                                break;
            default:            // setprecision 0 - 19
                                __precision__ = (int) manipulator;
                                sprintf (__floatFormat__, "%%.%if", __precision__);
                                sprintf (__doubleFormat__, "%%.%ilf", __precision__);
                                break;
        }
        return *this;
    }

    // explicit ostream class specializations for integers

    template<>
    ostream& ostream::operator << <int16_t> (const int16_t& value) {
        if (__showpoint__) {
            char buf [9]; // max/min value = 32767, -32768, one point possible
            sprintf (buf, "%i", value);
            if (value >= 1000 || value <= -1000) {
                int i = strlen (buf) + 1;
                for (int _ = 0; _ <= 3; _++)
                    buf [i--] = buf [i];
                #ifdef __LOCALE_HPP__
                    buf [i] = __locale_thousandSeparator__;
                #else
                    buf [i] = ',';
                #endif
            }
            Serial.print (buf);
        } else {
            Serial.print (value);
        }
        return *this;
    }

    template<>
    ostream& ostream::operator << <uint16_t> (const uint16_t& value) {
        if (__showpoint__) {
            char buf [8]; // max value = 65535, one point possible
            sprintf (buf, "%u", value);
            if (value >= 1000) {
                int i = strlen (buf) + 1;
                for (int _ = 0; _ <= 3; _++)
                    buf [i--] = buf [i];
                #ifdef __LOCALE_HPP__
                    buf [i] = __locale_thousandSeparator__;
                #else
                    buf [i] = ',';
                #endif
            }
            Serial.print (buf);
        } else {
            Serial.print (value);
        }
        return *this;
    }

    template<>
    ostream& ostream::operator << <int32_t> (const int32_t& value) {
        if (__showpoint__) {
            char buf [16]; // max/min value = 2147483647, -2147483648, 3 points possible
            sprintf (buf, "%li", value);
            int points = 0;
            if (value >= 1000 || value <= -1000) 
                points ++;
            if (value >= 1000000 || value <= -1000000) 
                points ++;
            if (value >= 1000000000 || value <= -1000000000) 
                points ++;
            if (points) {
                int n = strlen (buf) + points + 1;
                int i = n;
                while (i && points) {
                    if ((n - i) % 4 == 1 && (n - i) > 1) {
                        #ifdef __LOCALE_HPP__
                            buf [i] = __locale_thousandSeparator__;
                        #else
                            buf [i] = ',';
                        #endif
                        points--;
                    } else {
                        buf [i] = buf [i - points];
                    }
                    i--;
                }
            }
            Serial.print (buf);
        } else {
            Serial.print (value);
        }
        return *this;
    }

    template<>
    ostream& ostream::operator << <uint32_t> (const uint32_t& value) {
        if (__showpoint__) {
            char buf [15]; // max value = 4294967295, 3 points possible
            sprintf (buf, "%lu", value);
            int points = 0;
            if (value >= 1000) 
                points ++;
            if (value >= 1000000) 
                points ++;
            if (value >= 1000000000) 
                points ++;
            if (points) {
                int n = strlen (buf) + points + 1;
                int i = n;
                while (i && points) {
                    if ((n - i) % 4 == 1 && (n - i) > 1) {
                        #ifdef __LOCALE_HPP__
                            buf [i] = __locale_thousandSeparator__;
                        #else
                            buf [i] = ',';
                        #endif
                        points--;
                    } else {
                        buf [i] = buf [i - points];
                    }
                    i--;
                }
            }
            Serial.print (buf);
        } else {
            Serial.print (value);
        }
        return *this;
    }

    // explicit ostream class specializations for floats and doubles

    template<>
    ostream& ostream::operator << <float> (const float& value) {
        char buf [61]; // min float = -3.4028235×10^38 -> max 60 characters (considering max precision = 19)
        switch (__fpOutput__) {
            case defaultfloat:
                                #ifdef ARDUINO_ARCH_AVR
                                    dtostrf (value, 0, 2, buf); // default AVR format
                                #else
                                    sprintf (buf, "%f", value);                                    
                                #endif
                                break;
            case fixed:
                                #ifdef ARDUINO_ARCH_AVR
                                    dtostrf (value, 1, __precision__, buf);
                                #else
                                    sprintf (buf, __floatFormat__, value);
                                #endif
                                break;
        }

        // find ., e or end of string and apply local decimal separator
        int d = 0;
        for (int i = 0; ; i++) {
            if (buf [i] == '.') {
                #ifdef __LOCALE_HPP__
                    buf [i] = __locale_decimalSeparator__;
                #endif
                d = i;
                break;
            }
            else if (buf [i] == 0 || buf [i] > '9' ) {
                d = i;
                break;
            }
        }

        if (__showpoint__) {
            int m = d % 3;
            for (int i = 0; i < d; i++) {
                if (i > 0 && i < d - 1 && i % 3 == m) {
                    #ifdef __LOCALE_HPP__
                        Serial.print (__locale_thousandSeparator__);
                    #else
                        Serial.print (',');
                    #endif
                }
                Serial.print (buf [i]);
            }
            Serial.print (buf + d);
        } else {
            Serial.print (buf);
        }

        return *this;
    }

    //  - vendar je pri AVR ke 4 bytni

    template<>
    ostream& ostream::operator << <double> (const double& value) {
        const int bufSize = (sizeof(double) == 4) ? 61 : 331; // min double -1.7976931348623157×10^308 -> max 331 characters (considering max precision = 19)
        char buf [bufSize];
        switch (__fpOutput__) {
            case defaultfloat:
                                #ifdef ARDUINO_ARCH_AVR
                                    dtostrf (value, 0, 2, buf); // default AVR format
                                #else
                                    sprintf (buf, "%lf", value);                                    
                                #endif
                                break;
            case fixed:
                                #ifdef ARDUINO_ARCH_AVR
                                    dtostrf (value, 1, __precision__, buf);
                                #else
                                    sprintf (buf, __doubleFormat__, value);   
                                #endif
                                break;
        }

        // find ., e or end of string and apply local decimal separator
        int d = 0;
        for (int i = 0; ; i++) {
            if (buf [i] == '.') {
                #ifdef __LOCALE_HPP__
                    buf [i] = __locale_decimalSeparator__;
                #endif
                d = i;
                break;
            }
            else if (buf [i] == 0 || buf [i] > '9' ) {
                d = i;
                break;
            }
        }

        if (__showpoint__) {
            int m = d % 3;
            for (int i = 0; i < d; i++) {
                if (i > 0 && i < d - 1 && i % 3 == m) {
                    #ifdef __LOCALE_HPP__
                        Serial.print (__locale_thousandSeparator__);
                    #else
                        Serial.print (',');
                    #endif
                }
                Serial.print (buf [i]);
            }
            Serial.print (buf + d);
        } else {
            Serial.print (buf);
        }

        return *this;
    }
 


    // explicit ostream class specialization for time_t and struct tm
    #ifndef ARDUINO_ARCH_AVR // Assuming Arduino Mega or Uno
        template<>
        ostream& ostream::operator << <struct tm> (const struct tm& value) {
            char buf [80];
            #ifndef __LOCALE_HPP__
                strftime (buf, sizeof (buf), "%Y/%m/%d %T", &value);
            #else
                strftime (buf, sizeof (buf), __locale_time__, &value);
            #endif
            Serial.print (buf);            
            return *this;
        }

        template<>
        ostream& ostream::operator << <time_t> (const time_t& value) {
            struct tm st;
            localtime_r (&value, &st);
            return operator << (st);
        }
    #endif

    // explicit ostream class specialization for uth8char
    #ifdef __UTF8CHAR__
        template<>
        ostream& ostream::operator << <utf8char> (const utf8char& value) {
            utf8char u8 = value;

            char c = u8.__c_str__ [0] = value.__c_str__ [0];
            if ((c & 0x80) == 0) { // 1-byte character
                u8.__c_str__ [1] = 0;
            } else if ((c & 0xE0) == 0xC0) { // 2-byte character
                u8.__c_str__ [1] = value.__c_str__ [1];
                u8.__c_str__ [2] = 0;
            } else if ((c & 0xF0) == 0xE0) { // 3-byte character
                u8.__c_str__ [1] = value.__c_str__ [1];
                u8.__c_str__ [2] = value.__c_str__ [2];
                u8.__c_str__ [3] = 0;
            } else if ((c & 0xF8) == 0xF0) { // 4-byte character
                u8.__c_str__ [1] = value.__c_str__ [1];
                u8.__c_str__ [2] = value.__c_str__ [2];
                u8.__c_str__ [3] = value.__c_str__ [3];
                u8.__c_str__ [4] = 0;
            } else { // invalid UTF-8 character
                u8.__c_str__ [1] = 0; 
            }

            Serial.print (u8.__c_str__);
            return *this;
        }
    #endif


    // Create a working instances
    ostream cout;


    // istream
    class istream {

      private:

          char buf [__CONSOLE_BUFFER_SIZE__];

      public:

        // istream >> char
        istream& operator >> (char& value) {
            while (!Serial.available ()) delay (10);
            value = Serial.read ();            
            return *this;
        }        

        // istream >> int
        istream& operator >> (int& value) {
            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) delay (10);
                char c = Serial.read ();
                #ifndef __LOCALE_HPP__
                    if (c > ' ') {
                        buf [i] = c;
                        buf [++ i] = 0;
                    } else {
                        value = atoi (buf);
                        break;
                    }
                #else
                    if (c > ' ' && c != __locale_thousandSeparator__) {
                        buf [i] = c;
                        buf [++ i] = 0;
                    } else {
                        value = atoi (buf);
                        break;
                    }                
                #endif
            }
            return *this;
        } 

        // istream >> float
        istream& operator >> (float& value) {
            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) delay (10);
                char c = Serial.read ();
                if (c > ' ') {
                    buf [i] = c;
                    buf [++ i] = 0;
                } else {
                    #ifdef __LOCALE_HPP__
                        // replace decimal and thousand separators with '.' and ','
                        for (int i = 0; buf [i]; i++) {
                            if (buf [i] == __locale_decimalSeparator__)
                                buf [i] = '.';
                            else if (buf [i] == __locale_thousandSeparator__)
                                buf [i] = ',';
                        }
                    #endif
                    value = atof (buf);
                    break;
                }
            }
            return *this;
        } 

        // istream >> double
        istream& operator >> (double& value) {
            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) delay (10);
                char c = Serial.read ();
                if (c > ' ') {
                    buf [i] = c;
                    buf [++ i] = 0;
                } else {
                    #ifdef __LOCALE_HPP__
                        // replace decimal and thousand separators with '.' and ','
                        for (int i = 0; buf [i]; i++) {
                            if (buf [i] == __locale_decimalSeparator__)
                                buf [i] = '.';
                            else if (buf [i] == __locale_thousandSeparator__)
                                buf [i] = ',';
                        }
                    #endif
                    value = atof (buf);
                    break;
                }
            }
            return *this;
        } 

        // istream >> char * // warning, it doesn't chech buffer overflow
        istream& operator >> (char *value) {
            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) delay (10);
                char c = Serial.read ();
                if (c > ' ') {
                    buf [i] = c;
                    buf [++ i] = 0;
                } else {
                    strcpy (value, buf);
                    break;
                }
            }
            return *this;
        }
/*
        // istream >> String
        istream& operator >> (String& value) {
            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) delay (10);
                char c = Serial.read ();
                if (c > ' ') {
                    buf [i] = c;
                    buf [++ i] = 0;
                } else {
                    value = String (buf);
                    break;
                }
            }
            return *this;
        }

*/

        // istream >> any other class that has a constructor of type T (char *)
        template<typename T>
        istream& operator >> (T& value) {
            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) delay (10);
                char c = Serial.read ();
                if (c > ' ') {
                    buf [i] = c;
                    buf [++ i] = 0;
                } else {
                    value = T (buf);
                    break;
                }
            }
            return *this;
        }

    };

    // Create a working instnces
    istream cin;

#endif
