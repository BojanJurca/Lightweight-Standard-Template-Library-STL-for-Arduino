/*
 *  ostream.hpp for Arduino
 *
 *  This file is part of cin, cout library for Arduino: https://github.com/BojanJurca/cin-cout-for-Arduino
 *
 *  Oct 23, 2025, Bojan Jurca
 *
 */


#ifndef __OSTREAM_HPP__
    #define __OSTREAM_HPP__


    // ----- CODE -----


    // Serial initialization
    #ifndef __CINIT__
        #define __CINIT__

        #ifdef ARDUINO_ARCH_AVR 
            void cinit (bool waitForSerial = false, unsigned int waitAfterSerial = 100, unsigned int serialSpeed = 9600) {
                Serial.begin (serialSpeed);
                if (waitForSerial)
                    while (!Serial) 
                        delay (10);
                delay (waitAfterSerial);
            }
        #else
            void cinit (bool waitForSerial = false, unsigned int waitAfterSerial = 100, unsigned int serialSpeed = 115200) {
                Serial.begin (serialSpeed);
                if (waitForSerial)
                    while (!Serial) 
                        delay (10);
                delay (waitAfterSerial);
            }
        #endif
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
            char __longDoubleFormat__ [7] = "%.6Lf";        // for default precision

        public:

            ostream& operator << (const char* value) {
                Serial.print (value);
                return *this;
            }

            template<size_t N>
            ostream& operator << (const char (&value) [N]) {
                return *this << static_cast<const char *> (value);
            }

            template<typename T>
            ostream& operator << (const T& value) {
                Serial.print (value);            
                return *this;
            }

        private:

            void __showPointPrintInt__ (char *buf, int len) {
                int m = (len + 2) % 3;
                for (int i = 0; i < len; ++i) {
                    Serial.print (buf [i]);
                    // print separator every 3 characters
                    if (i % 3 == m && i < len - 1) {
                        #ifdef __LOCALE_HPP__
                            Serial.print (lc_numeric_locale->getThousandsSeparator ());
                        #else
                            Serial.print (',');
                        #endif
                    }
                }
            }

            void __showPointPrintFloat__ (char *buf) {
                for (int i = 0;; i++) {
                    switch (buf [i]) {
                        case '.':   // decimal separator reached
                                    __showPointPrintInt__ (buf, i);
                                    #ifdef __LOCALE_HPP__
                                        Serial.print (lc_numeric_locale->getDecimalSeparator ());
                                    #else
                                        Serial.print ('.');
                                    #endif
                                    Serial.print (&buf [i + 1]);
                                    return;
                        case 0:     // end of string reached, decimal separator was not present
                                    __showPointPrintInt__ (buf, i);
                                    return;
                    }
                }
            }

            #ifdef __LOCALE_HPP__
                void __localizeSeparators__ (char *buf) {
                    for (int i = 0; buf [i]; i++) {
                        switch (buf [i]) {
                            case '.':   // decimal separator
                                        buf [i] = lc_numeric_locale->getDecimalSeparator ();
                                        break;
                            case ',':   // thousands separator
                                        buf [i] = lc_numeric_locale->getThousandsSeparator ();
                                        break;
                        }
                    }
                }
            #endif

    };


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
                                if (manipulator >= 0 && manipulator <= 19) {
                                    __precision__ = (int) manipulator - precision0;
                                    sprintf (__floatFormat__, "%%.%if", __precision__);
                                    sprintf (__doubleFormat__, "%%.%ilf", __precision__);
                                    sprintf (__longDoubleFormat__, "%%.%iLf", __precision__);
                                }
                                break;
        }
        return *this;
    }

    // explicit ostream class specializations for integer data types

    template<>
    ostream& ostream::operator << <int16_t> (const int16_t& value) {
        if (__showpoint__) {
            char buf [7]; // max: -32768, min: 32767
            __showPointPrintInt__ (buf, sprintf (buf, "%i", value));
        } else {
            Serial.print (value);
        }
        return *this;
    }

    template<>
    ostream& ostream::operator << <uint16_t> (const uint16_t& value) {
        if (__showpoint__) {
            char buf [6]; // max: 65535
            __showPointPrintInt__ (buf, sprintf (buf, "%u", value));
        } else {
            Serial.print (value);
        }
        return *this;
    }

    template<>
    ostream& ostream::operator << <int32_t> (const int32_t& value) {
        if (__showpoint__) {
            char buf [12]; // min: -2147483648, max: 2147483647
            __showPointPrintInt__ (buf, sprintf (buf, "%li", value));
        } else {
            Serial.print (value);
        }
        return *this;
    }

    template<>
    ostream& ostream::operator << <uint32_t> (const uint32_t& value) {
        if (__showpoint__) {
            char buf [11]; // max: 4294967295
            __showPointPrintInt__ (buf, sprintf (buf, "%lu", value));
        } else {
            Serial.print (value);
        }
        return *this;
    }

    #ifdef ARDUINO_ARCH_AVR
        template<>
        ostream& ostream::operator << <uint64_t> (const uint64_t& value) {
            if (value == 0) {
                Serial.print ('0');
                return *this;
            }

            uint64_t val = value;
            char buf [21]; // max: 18446744073709551615
            int i = 20;
            buf [i --] = 0;
            while (val > 0) {
                buf [i--] = '0' + (val % 10);
                val /= 10;
            }
            if (__showpoint__)
                __showPointPrintInt__ (&buf [i + 1], 20 - i - 1);
            else
                Serial.print (&buf [i + 1]);

            return *this;
        }

        template<>
        ostream& ostream::operator << <int64_t> (const int64_t& value) {
            if (value < 0) {
                Serial.print ('-');
                ostream::operator << <uint64_t> ((uint64_t) (-(value + 1)) + 1); // convert to uint64_t (considering possible overflow)
            } else {
                ostream::operator << <uint64_t> ((uint64_t) value);
            }
            return *this;
        }
    #else
        template<>
        ostream& ostream::operator << <int64_t> (const int64_t& value) {
            if (__showpoint__) {
                char buf [21]; // min: -9223372036854775808, max: 9223372036854775807
                __showPointPrintInt__ (buf, sprintf (buf, "%lli", value));
            } else {
                Serial.print (value);
            }
            return *this;
        }

        template<>
        ostream& ostream::operator << <uint64_t> (const uint64_t& value) {
            if (__showpoint__) {
                char buf [21]; // max: 18446744073709551615
                __showPointPrintInt__ (buf, sprintf (buf, "%llu", value));
            } else {
                Serial.print (value);
            }
            return *this;
        }
    #endif

    // explicit ostream class specializations for floats and doubles

    template<>
    ostream& ostream::operator << <float> (const float& value) {
        char buf [61]; // min: -3.4028235×10^38, max 60 characters (considering max precision = 19)
        if (__fpOutput__ == fixed) {
            #ifdef ARDUINO_ARCH_AVR
                dtostrf (value, 1, __precision__, buf);
            #else
                snprintf (buf, sizeof (buf), __floatFormat__, value);
            #endif
            if (__showpoint__ && __fpOutput__ == fixed) {
                __showPointPrintFloat__ (buf);
                return *this;
            }
        } else {
            #ifdef ARDUINO_ARCH_AVR
                dtostrf (value, 0, 2, buf); // default AVR format
            #else
                sprintf (buf, "%f", value);                                    
            #endif
        }
        #ifdef __LOCALE_HPP__
            __localizeSeparators__ (buf);
        #endif
        Serial.print (buf);

        return *this;
    }

    template<>
    ostream& ostream::operator << <double> (const double& value) {
        const int bufSize = (sizeof (double) == 4 /* only 4 bytes on AVR boards */) ? 61 : 331; // min: -1.7976931348623157×10^308 -> max cca 4932 characters (considering max precision = 19)
        char buf [bufSize];
        if (__fpOutput__ == fixed) {
            #ifdef ARDUINO_ARCH_AVR
                dtostrf (value, 1, __precision__, buf);
            #else
                snprintf (buf, bufSize, __doubleFormat__, value);
            #endif
            if (__showpoint__ && __fpOutput__ == fixed) {
                __showPointPrintFloat__ (buf);
                return *this;
            }
        } else {
            #ifdef ARDUINO_ARCH_AVR
                dtostrf (value, 0, 2, buf); // default AVR format
            #else
                sprintf (buf, "%f", value);                                    
            #endif
        }
        #ifdef __LOCALE_HPP__
            __localizeSeparators__ (buf);
        #endif
        Serial.print (buf);

        return *this;
    }
 
    template<>
    ostream& ostream::operator << <long double> (const long double& value) {
        const int bufSize = (sizeof (long double) == 4 /* only 4 bytes on AVR boards */) ? 61 : 331; // min: -1.7976931348623157×10^308 -> max 331 characters (considering max precision = 19)
        char buf [bufSize];
        if (__fpOutput__ == fixed) {
            #ifdef ARDUINO_ARCH_AVR
                dtostrf (value, 1, __precision__, buf);
            #else
                snprintf (buf, bufSize, __longDoubleFormat__, value);
            #endif
            if (__showpoint__ && __fpOutput__ == fixed) {
                __showPointPrintFloat__ (buf);
                return *this;
            }
        } else {
            #ifdef ARDUINO_ARCH_AVR
                dtostrf (value, 0, 2, buf); // default AVR format
            #else
                sprintf (buf, "%f", value);                                    
            #endif
        }
        #ifdef __LOCALE_HPP__
            __localizeSeparators__ (buf);
        #endif
        Serial.print (buf);

        return *this;
    }


    // explicit ostream class specialization for time_t and struct tm
    #ifndef ARDUINO_ARCH_AVR 
        template<>
        ostream& ostream::operator << <struct tm> (const struct tm& value) {
            char buf [80];
            #ifndef __LOCALE_HPP__
                strftime (buf, sizeof (buf), "%Y/%m/%d %T", &value);
            #else
                strftime (buf, sizeof (buf), lc_time_locale->getTimeFormat (), &value);
            #endif
            Serial.print (buf);            
            return *this;
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

#endif
