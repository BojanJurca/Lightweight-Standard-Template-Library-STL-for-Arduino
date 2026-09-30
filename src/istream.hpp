/*
 *  istream.hpp for Arduino
 *
 *  This file is part of Lightweight C++ Standard Template Library (STL) for Arduino: https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino
 *
 *  Oct 10, 2026, Bojan Jurca
 *
 */


#ifndef __ISTREAM_HPP__
    #define __ISTREAM_HPP__

    #include <iostream_common.hpp>


    // ----- TUNNING PARAMETERS -----

    #define __CONSOLE_BUFFER_SIZE__ 64 // max 63 characters in internal buffer


    // ----- CODE -----


    class istream {

      private:

          char buf [__CONSOLE_BUFFER_SIZE__];

      public:

        // istream >> char
        inline istream& operator >> (char& value) {
            __checkIostreamInitialized__ ();

            while (!Serial.available ()) 
                delay (10);
            value = Serial.read ();            
            return *this;
        }

        // istream >> int
        inline istream& operator >> (int& value) {
            __checkIostreamInitialized__ ();

            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ())
                    delay (10);
                char c = Serial.read ();
                #ifndef __LOCALE_HPP__
                    if (c > ' ' && c <= '9' && c != '.') {
                        buf [i] = c;
                        buf [++ i] = 0;
                    } else {
                        value = atoi (buf);
                        break;
                    }
                #else
                    if (c > ' ' && c <= '9' && c != lc_numeric_locale->getDecimalSeparator ()) {
                        buf [i] = c;
                        buf [++ i] = 0;
                    } else {
                        __unLocalizeSeparators__ (buf);
                        value = atoi (buf);
                        break;
                    }                
                #endif
            }
            return *this;
        } 

        // istream >> long
        inline istream& operator >> (long& value) {
            __checkIostreamInitialized__ ();

            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ())
                    delay (10);
                char c = Serial.read ();
                #ifndef __LOCALE_HPP__
                    if (c > ' ' && c <= '9' && c != '.') {
                        buf [i] = c;
                        buf [++ i] = 0;
                    } else {
                        value = atol (buf);
                        break;
                    }
                #else
                    if (c > ' ' && c <= '9' && c != lc_numeric_locale->getDecimalSeparator ()) {
                        buf [i] = c;
                        buf [++ i] = 0;
                    } else {
                        __unLocalizeSeparators__ (buf);
                        value = atol (buf);
                        break;
                    }                
                #endif
            }
            return *this;
        } 

        // istream >> float
        istream& operator >> (float& value) {
            __checkIostreamInitialized__ ();

            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) 
                    delay (10);
                char c = Serial.read ();
                if (c > ' ') {
                    buf [i] = c;
                    buf [++ i] = 0;
                } else {
                    __unLocalizeSeparators__ (buf);
                    value = atof (buf);
                    break;
                }
            }
            return *this;
        } 

        // istream >> double
        inline istream& operator >> (double& value) {
            __checkIostreamInitialized__ ();

            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) 
                    delay (10);
                char c = Serial.read ();
                if (c > ' ') {
                    buf [i] = c;
                    buf [++ i] = 0;
                } else {
                    __unLocalizeSeparators__ (buf);
                    value = atof (buf);
                    break;
                }
            }
            return *this;
        } 

        // istream >> char * // warning, it doesn't chech buffer overflow
        inline istream& operator >> (char *value) {
            __checkIostreamInitialized__ ();

            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) 
                    delay (10);
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

        // istream >> any other class that has a constructor of type T (char *)
        template<typename T>
        inline istream& operator >> (T& value) {
            __checkIostreamInitialized__ ();
            
            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) 
                    delay (10);
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

        private:

            inline void __unLocalizeSeparators__ (char *buf) {
                #ifdef __LOCALE_HPP__
                    char thousandsSeparator = lc_numeric_locale->getThousandsSeparator ();
                    char decimalSeparator = lc_numeric_locale->getDecimalSeparator ();
                #else
                    char thousandsSeparator = ',';
                    char decimalSeparator = '.';
                #endif

                int i = 0; 
                for (int j = 0; buf [j]; j++)
                    if (buf [j] == decimalSeparator)
                        buf [i ++] = '.';
                    else if (buf [j] != thousandsSeparator)
                        buf [i ++] = buf [j];
                buf [i] = 0;
            }

    };

    // Create a working singleton instnces
    inline istream& __getCinInstance__ () {
        static istream instance;
        return instance;
    }

    #if __cplusplus >= 201703L
        inline istream& cin = __getCinInstance__ ();
    #else
        static istream& cin = __getCinInstance__ ();
    #endif


#endif
