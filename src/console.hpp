/*
 *  console.hpp for Arduino (ESP boards)
 * 
 *  This file is part of Lightweight C++ Standard Template Library (STL) for Arduino: https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino
 * 
 *  March 12, 2025, Bojan Jurca
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


    // cout
    #define endl "\r\n"

    class Cout {

      public:

        template<typename T>
        Cout& operator << (const T& value) {
            Serial.print (value);            
            return *this;
        }

        #ifdef __CSTRING_HPP__
            template<size_t N>
            Cout& operator << (const Cstring<N>& value) {
                Serial.print ((char *) &value);
                return *this;
            }
        #endif

        #ifdef __LIST_HPP__
            template<typename T>
            Cout& operator << (list<T>& value) {
                Serial.print ("FRONT→");
                for (auto e: value) {
                    this->operator<<(e);
                    Serial.print ("→");
                }
                Serial.print ("NULL");
                return *this;
            }
        #endif

        #ifdef __VECTOR_HPP__
            template<typename T>
            Cout& operator << (vector<T>& value) {
                bool first = true;
                Serial.print ("[");
                for (auto e: value) {
                    if (!first)
                        Serial.print (",");
                    first = false;
                    this->operator<<(e);
                }
                Serial.print ("]");
                return *this;
            }
        #endif

        #ifdef __QUEUE_HPP__
            template<typename T>
            Cout& operator << (queue<T>& value) {
                bool first = true;
                Serial.print ("[");
                for (auto e: value) {
                    if (!first)
                        Serial.print (",");
                    first = false;
                    this->operator<<(e);
                }
                Serial.print ("]");
                return *this;
            }
        #endif

        #ifdef __MAP_HPP__
            template<typename K, typename V>
            Cout& operator << (Map<K, V>& value) {
                bool first = true;
                Serial.print ("[");
                for (auto e: value) {
                    if (!first)
                        Serial.print (",");
                    first = false;
                    this->operator<<(e.first); 
                    Serial.print ("→");
                    this->operator<<(e.second);
                }
                Serial.print ("]");
                return *this;
            }
        #endif

    };

    // explicit Cout class specializations for floats and doubles when locale is used

    #ifdef __LOCALE_HPP__
        template<>
        Cout& Cout::operator << <float> (const float& value) {
            char buf [32];
            #ifndef ARDUINO_ARCH_AVR
                sprintf (buf, "%f", value);
            #else
                dtostrf (value, 1, 6, buf);
            #endif
            // replace decimal and thousand separators with '.' and ','
            for (int i = 0; buf [i]; i++) {
                if (buf [i] == '.')
                    buf [i] = __locale_decimalSeparator__;
                else if (buf [i] == ',')
                    buf [i] = __locale_thousandSeparator__;
            }
            Serial.print (buf);            
            return *this;
        }

        template<>
        Cout& Cout::operator << <double> (const double& value) {
            char buf [32];
            #ifndef ARDUINO_ARCH_AVR
                sprintf (buf, "%lf", value);
            #else
                dtostrf (value, 1, 6, buf);
            #endif
            // replace decimal and thousand separators with '.' and ','
            for (int i = 0; buf [i]; i++) {
                if (buf [i] == '.')
                    buf [i] = __locale_decimalSeparator__;
                else if (buf [i] == ',')
                    buf [i] = __locale_thousandSeparator__;
            }
            Serial.print (buf);            
            return *this;
        }
    #endif

    // explicit Cout class specialization for time_t and struct tm
    #ifndef ARDUINO_ARCH_AVR // Assuming Arduino Mega or Uno
        template<>
        Cout& Cout::operator << <struct tm> (const struct tm& value) {
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
        Cout& Cout::operator << <time_t> (const time_t& value) {
            struct tm st;
            localtime_r (&value, &st);
            return operator << (st);
        }
    #endif

    // explicit Cout class specialization for uth8char
    template<>
    Cout& Cout::operator << <utf8char> (const utf8char& value) {
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


    // Create a working instances
    Cout cout;


    // cin
    class Cin {

      private:

          char buf [__CONSOLE_BUFFER_SIZE__];

      public:

        // Cin >> char
        Cin& operator >> (char& value) {
            while (!Serial.available ()) delay (10);
            value = Serial.read ();            
            return *this;
        }        

        // Cin >> int
        Cin& operator >> (int& value) {
            buf [0] = 0;
            int i = 0;
            while (i < __CONSOLE_BUFFER_SIZE__ - 1) {
                while (!Serial.available ()) delay (10);
                char c = Serial.read ();
                if (c > ' ') {
                    buf [i] = c;
                    buf [++ i] = 0;
                } else {
                    value = atoi (buf);
                    break;
                }
            }
            return *this;
        } 

        // Cin >> float
        Cin& operator >> (float& value) {
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

        // Cin >> double
        Cin& operator >> (double& value) {
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

        // Cin >> char * // warning, it doesn't chech buffer overflow
        Cin& operator >> (char *value) {
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

        // Cin >> String
        Cin& operator >> (String& value) {
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


        // Cin >> any other class that has a constructor of type T (char *)
        template<typename T>
        Cin& operator >> (T& value) {
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
    Cin cin;

#endif
