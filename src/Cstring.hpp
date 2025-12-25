/*
 *  Cstring.hpp for Arduino
 * 
 *  This file is part of Lightweight C++ Standard Template Library (STL) for Arduino: https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino
 * 
 *  Dec 25, 2025 Bojan Jurca
 *  
 */


#ifndef __CSTRING_HPP__
    #define __CSTRING_HPP__


    // ----- TUNNING PARAMETERS -----

    #ifndef cstring
        #define cstring Cstring<300>
    #endif


    // error flags: there are only two types of error flags that can be set: err_overflow and err_out_of_range- please note that all errors are negative (char) numbers
    #define err_ok           ((signed char) 0b00000000) //    0 - no error    
    #define err_overflow     ((signed char) 0b10000001) // -127 - buffer overflow
    #define err_out_of_range ((signed char) 0b10000010) // -126 - invalid index

    static const size_t npos = (size_t) 0xFFFFFFFFFFFFFFFF;


    // fixed size string, actually C char arrays with additional functions and operators
    template<size_t N> struct Cstring {

        private: 

            // internal storage: char array (= 0-terminated C string)
            char __c_str__ [N + 2] = {}; // add 2 characters at the end __c_str__ [N] will detect if string gets too long (owerflow), __c_str__ [N + 1] will guard the end of the string and will always be 0, initialize it with zeros
            signed char __errorFlags__ = 0;

        public:

            signed char errorFlags () const { return __errorFlags__ & 0b01111111; }
            void clearErrorFlags () { __errorFlags__ = 0; }
        
        
            // constructors
            Cstring () {}                                      // for declarations like Cstring<15> a;

            Cstring (const char *other) {                      // for declarations with initialization, like Cstring<15> b ("abc");
                if (other) {                                                  // check if NULL char * pointer to overcome from possible programmer's errors
                    strncpy (this->__c_str__, other, N + 1);
                    if (this->__c_str__ [N]) {
                         __errorFlags__ = err_overflow;                       // err_overflow
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                        this->__rTrimUnfinishedUtf8Character__ ();
                    }
                }
            }

            Cstring (const Cstring& other) {                  // for declarations with initialization, like Cstring<15> c = a;            
                strncpy (this->__c_str__, other.__c_str__, N + 1);
                this->__errorFlags__ = other.__errorFlags__;                  // inherit all errors from other Cstring
            }

            template<size_t M>
            Cstring (const Cstring<M>& other) {
                strncpy (this->__c_str__, other.c_str (), N + 1);
                this->__errorFlags__ = other.errorFlags ();
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= err_overflow;
                    this->__c_str__ [N] = 0;
                    this->__rTrimUnfinishedUtf8Character__ ();
                }
            }            
                    
            Cstring (const char& other) {                      // for declarations with initialization, like Cstring<15> d ('c'); (convert char to Cstring)
                #if N > 0
                    this->__c_str__ [0] = other;
                    this->__c_str__ [1] = 0;
                    if (this->__c_str__ [N]) {
                        __errorFlags__ = err_overflow;                            // err_overflow
                        this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                    }
                #else
                    this->__c_str__ [0] = 0;
                    __errorFlags__ = err_overflow;                                // err_overflow
                #endif
            }

            #ifdef __LOCALE_HPP__
                Cstring (const utf8char& other) {               // for declarations with initialization, like Cstring<15> d (utf8char ("🌎")); (convert utf8charchar to Cstring)
                    if ((other.c_str [0] & 0x80) == 0 && N > 0) {             // 0xxxxxxx = 1 byte character
                        this->__c_str__ [0] = other.c_str [0];          
                        this->__c_str__ [1] = 0; 
                    } else if ((other.c_str [0] & 0xE0) == 0xC0 && N > 1) {   // 110xxxxx = 2 byte character
                        this->__c_str__ [0] = other.c_str [0];          
                        this->__c_str__ [1] = other.c_str [1];
                        this->__c_str__ [2] = 0; 
                    } else if ((other.c_str [0] & 0xF0) == 0xE0 && N > 2) {   // 1110xxxx = 3 byte character
                        this->__c_str__ [0] = other.c_str [0];
                        this->__c_str__ [1] = other.c_str [1];
                        this->__c_str__ [2] = other.c_str [2];
                        this->__c_str__ [3] = 0; 
                    } else if ((other.c_str [0] & 0xF8) == 0xF0 && N > 3) {   // 11110xxx = 4 byte character
                        this->__c_str__ [0] = other.c_str [0];
                        this->__c_str__ [1] = other.c_str [1];
                        this->__c_str__ [2] = other.c_str [2];
                        this->__c_str__ [3] = other.c_str [3];
                        this->__c_str__ [4] = 0; 
                    } else {                                            // overflow
                        this->__c_str__ [0] = 0;
                        __errorFlags__ = err_overflow;                       // err_overflow
                    }
                }
            #endif
            
            Cstring (int number) {                             // for declarations with initialization, like Cstring<15> e (3); (convert int to Cstring)
                snprintf (this->__c_str__, N + 2, "%i", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = err_overflow;                            // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            Cstring (unsigned int number) {                    // for declarations with initialization, like Cstring<15> e (3); (convert unsigned int to Cstring)
                snprintf (this->__c_str__, N + 2, "%u", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = err_overflow;                            // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            Cstring (long number) {                            // for declarations with initialization, like Cstring<15> e (3); (convert long to Cstring)
                snprintf (this->__c_str__, N + 2, "%ld", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = err_overflow;                            // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            Cstring (unsigned long number) {                   // for declarations with initialization, like Cstring<15> e (3); (convert unsigned long to Cstring)
                snprintf (this->__c_str__, N + 2, "%lu", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = err_overflow;                            // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            // this doesn't work on AVR boards
            #ifndef ARDUINO_ARCH_AVR
                Cstring (long long number) {                       // for declarations with initialization, like Cstring<15> e (3); (convert long to Cstring)
                    snprintf (this->__c_str__, N + 2, "%lld", number);
                    if (this->__c_str__ [N]) {
                        __errorFlags__ = err_overflow;                            // err_overflow
                        this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                    }
                }

                Cstring (unsigned long long number) {              // for declarations with initialization, like Cstring<15> e (3); (convert unsigned long to Cstring)
                    snprintf (this->__c_str__, N + 2, "%llu", number);
                    if (this->__c_str__ [N]) {
                        __errorFlags__ = err_overflow;                            // err_overflow
                        this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                    }
                }
            #endif

            Cstring (float number) {                           // for declarations with initialization, like Cstring<15> e (3.1); (convert float to Cstring)
                #ifdef ARDUINO_ARCH_AVR
                    dtostrf (number, -(N + 1), 2, this->__c_str__);
                #else
                    snprintf (this->__c_str__, N + 2, "%f", number);
                #endif
                #ifdef __LOCALE_HPP__
                    // replace decimal and thousand separators with locale settings
                    for (int i = 0; this->__c_str__ [i]; i++) {
                        if (this->__c_str__ [i] == '.')
                            this->__c_str__ [i] = lc_numeric_locale->getDecimalSeparator ();
                        else if (this->__c_str__ [i] == ',')
                            this->__c_str__ [i] = lc_numeric_locale->getThousandsSeparator ();
                    }
                #endif                
                if (this->__c_str__ [N]) {
                    __errorFlags__ = err_overflow;                            // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            Cstring (double number) {                          // for declarations with initialization, like Cstring<15> e (3.1); (convert float to Cstring)
                #ifdef ARDUINO_ARCH_AVR
                    dtostrf (number, -(N + 1), 2, this->__c_str__);
                #else
                    snprintf (this->__c_str__, N + 2, "%lf", number);
                #endif
                #ifdef __LOCALE_HPP__
                    // replace decimal and thousand separators with locale settings
                    for (int i = 0; this->__c_str__ [i]; i++) {
                        if (this->__c_str__ [i] == '.')
                            this->__c_str__ [i] = lc_numeric_locale->getDecimalSeparator ();
                        else if (this->__c_str__ [i] == ',')
                            this->__c_str__ [i] = lc_numeric_locale->getThousandsSeparator ();
                    }
                #endif                
                if (this->__c_str__ [N]) {
                    __errorFlags__ = err_overflow;                            // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            Cstring (long double number) {                     // for declarations with initialization, like Cstring<15> e (3.1); (convert float to Cstring)
                #ifdef ARDUINO_ARCH_AVR
                    dtostrf (number, -(N + 1), 2, this->__c_str__);
                #else
                    snprintf (this->__c_str__, N + 2, "%llf", number);
                #endif
                #ifdef __LOCALE_HPP__
                    // replace decimal and thousand separators with locale settings
                    for (int i = 0; this->__c_str__ [i]; i++) {
                        if (this->__c_str__ [i] == '.')
                            this->__c_str__ [i] = lc_numeric_locale->getDecimalSeparator ();
                        else if (this->__c_str__ [i] == ',')
                            this->__c_str__ [i] = lc_numeric_locale->getThousandsSeparator ();
                    }
                #endif                
                if (this->__c_str__ [N]) {
                    __errorFlags__ = err_overflow;                            // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            #ifndef ARDUINO_ARCH_AVR 
                Cstring (struct tm st) {                                // for declarations with initialization, like Cstring<15> t (st); (convert struct tm to Cstring)
                    char buf [80];
                    #ifdef __LOCALE_HPP__
                        strftime (buf, sizeof (buf), lc_time_locale->getTimeFormat (), &st);
                    #else
                        strftime (buf, sizeof (buf), "%Y/%m/%d %T", &st);
                    #endif

                    strncpy (this->__c_str__, buf, N + 1);
                    if (this->__c_str__ [N]) {
                        __errorFlags__ = err_overflow;                          // err_overflow
                        this->__c_str__ [N] = 0;                                // mark the end of the string regardles OVERFLOW
                    }
                }
            #endif


            // char *() operator so that Cstring can be used the same way as C char arrays, like; Cstring<5> a = "123"; Serial.printf ("%s\n", a);
            inline operator char *() const __attribute__((always_inline)) { return (char *) __c_str__; }
            // inline operator const char *() const __attribute__((always_inline)) { return (const char *) __c_str__; }


            // = operator
            Cstring operator = (const char *other) {           // for assigning C char array to Cstring, like: a = "abc";
                if (other) {                                                // check if NULL char * pointer to overcome from possible programmers' errors
                    this->__errorFlags__ = 0;                               // clear error flags
                    strncpy (this->__c_str__, other, N + 1);
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ = err_overflow;                // err_overflow
                        this->__c_str__ [N] = 0;                            // mark the end of the string regardles OVERFLOW
                        this->__rTrimUnfinishedUtf8Character__ ();
                    }
                }
                return *this;
            }
    
            Cstring operator = (const Cstring& other) {       // for assigning other Cstring to Cstring, like: a = b;
                if (this != &other) {
                    this->__errorFlags__ = 0;                               // clear error flags
                    strncpy (this->__c_str__, other.__c_str__, N + 1);
                    this->__errorFlags__ = other.__errorFlags__;            // inherit all errors from original string
                }
                return *this;
            }

            template<size_t M>
            Cstring operator = (const Cstring<M>& other) {    // for assigning other Cstring to Cstring of different size, like: a = b;
                strncpy (this->__c_str__, other.c_str (), N + 1);
                this->__errorFlags__ = other.errorFlags ();                 // inherit all errors from original string
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= err_overflow;                    // err_overflow
                    this->__c_str__ [N] = 0;                                // mark the end of the string regardles OVERFLOW
                    this->__rTrimUnfinishedUtf8Character__ ();
                }
                return *this;
            }

            Cstring operator = (const char& other) {           // for assigning character to Cstring, like: a = 'b';
                this->__errorFlags__ = 0;                                   // clear error flags
                #if N > 0
                    this->__c_str__ [0] = other; 
                    this->__c_str__ [1] = 0;                                // mark the end of the string
                #else
                    this->__c_str__ [0] = 0; 
                    this->__errorFlags__ = err_overflow;                    // err_overflow
                #endif
                return *this;
            }

            #ifdef __LOCALE_HPP__
                 Cstring operator = (const utf8char& other) {  // for assigning character to Cstring, like Cstring<15> d  = utf8char ("🌎"));
                    if ((other.c_str [0] & 0x80) == 0 && N > 0) {             // 0xxxxxxx = 1 byte character
                        this->__c_str__ [0] = other.c_str [0];          
                        this->__c_str__ [1] = 0;
                    } else if ((other.c_str [0] & 0xE0) == 0xC0 && N > 1) {   // 110xxxxx = 2 byte character
                        this->__c_str__ [0] = other.c_str [0];          
                        this->__c_str__ [1] = other.c_str [1];
                        this->__c_str__ [2] = 0;
                    } else if ((other.c_str [0] & 0xF0) == 0xE0 && N > 2) {   // 1110xxxx = 3 byte character
                        this->__c_str__ [0] = other.c_str [0];
                        this->__c_str__ [1] = other.c_str [1];
                        this->__c_str__ [2] = other.c_str [2];
                        this->__c_str__ [3] = 0;
                    } else if ((other.c_str [0] & 0xF8) == 0xF0 && N > 3) {   // 11110xxx = 4 byte character
                        this->__c_str__ [0] = other.c_str [0];
                        this->__c_str__ [1] = other.c_str [1];
                        this->__c_str__ [2] = other.c_str [2];
                        this->__c_str__ [3] = other.c_str [3];
                        this->__c_str__ [4] = 0;
                    } else {                                            // overflow
                        this->__c_str__ [0] = 0;
                        __errorFlags__ = err_overflow;                       // err_overflow
                    }
                    return *this;
                }
            #endif

            Cstring operator = (const int number) {                   // for assigning int to Cstring, like: a = 1234;
                snprintf (this->__c_str__, N + 2, "%i", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = err_overflow;                      // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            Cstring operator = (const unsigned int number) {           // for assigning unsigned int to Cstring, like: a = 1234;
                snprintf (this->__c_str__, N + 2, "%u", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = err_overflow;                      // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles the OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            Cstring operator = (const long number) {                   // for assigning long to Cstring, like: a = 1234;
                snprintf (this->__c_str__, N + 2, "%ld", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = err_overflow;                      // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            Cstring operator = (const unsigned long number) {          // for assigning unsigned long to Cstring, like: a = 1234;
                snprintf (this->__c_str__, N + 2, "%lu", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = err_overflow;                      // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            // this doesn't work on AVR boards
            #ifndef ARDUINO_ARCH_AVR
                Cstring operator = (const long long number) {              // for assigning long to Cstring, like: a = 1234;
                    snprintf (this->__c_str__, N + 2, "%lld", number);
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ = err_overflow;                      // err_overflow
                        this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                    } else {
                        this->__errorFlags__ = 0;                                 // clear error after new assignment
                    }
                    return *this;
                }

                Cstring operator = (const unsigned long long number) {     // for assigning unsigned long to Cstring, like: a = 1234;
                    snprintf (this->__c_str__, N + 2, "%lu", number);
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ = err_overflow;                      // err_overflow
                        this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                    } else {
                        this->__errorFlags__ = 0;                                 // clear error after new assignment
                    }
                    return *this;
                }
            #endif

            Cstring operator = (const float number) {                  // for assigning float to Cstring, like: a = 1234.5;
                #ifdef ARDUINO_ARCH_AVR
                    dtostrf (number, -(N + 1), 2, this->__c_str__);
                #else
                    snprintf (this->__c_str__, N + 2, "%f", number);
                #endif
                #ifdef __LOCALE_HPP__
                    // replace decimal and thousand separators with locale settings
                    for (int i = 0; this->__c_str__ [i]; i++) {
                        if (this->__c_str__ [i] == '.')
                            this->__c_str__ [i] = lc_numeric_locale->getDecimalSeparator ();
                        else if (this->__c_str__ [i] == ',')
                            this->__c_str__ [i] = lc_numeric_locale->getThousandsSeparator ();
                    }
                #endif                
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = err_overflow;                      // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVEFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            Cstring operator = (const double number) {                 // for assigning double to Cstring, like: a = 1234.5;
                #ifdef ARDUINO_ARCH_AVR
                    dtostrf (number, -(N + 1), 2, this->__c_str__);
                #else
                    snprintf (this->__c_str__, N + 2, "%llf", number);
                #endif
                #ifdef __LOCALE_HPP__
                    // replace decimal and thousand separators with locale settings
                    for (int i = 0; this->__c_str__ [i]; i++) {
                        if (this->__c_str__ [i] == '.')
                            this->__c_str__ [i] = lc_numeric_locale->getDecimalSeparator ();
                        else if (this->__c_str__ [i] == ',')
                            this->__c_str__ [i] = lc_numeric_locale->getThousandsSeparator ();
                    }
                #endif                
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = err_overflow;                      // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            Cstring operator = (const long double number) {            // for assigning double to Cstring, like: a = 1234.5;
                #ifdef ARDUINO_ARCH_AVR
                    dtostrf (number, -(N + 1), 2, this->__c_str__);
                #else
                    snprintf (this->__c_str__, N + 2, "%llf", number);
                #endif
                #ifdef __LOCALE_HPP__
                    // replace decimal and thousand separators with locale settings
                    for (int i = 0; this->__c_str__ [i]; i++) {
                        if (this->__c_str__ [i] == '.')
                            this->__c_str__ [i] = lc_numeric_locale->getDecimalSeparator ();
                        else if (this->__c_str__ [i] == ',')
                            this->__c_str__ [i] = lc_numeric_locale->getThousandsSeparator ();
                    }
                #endif                
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = err_overflow;                      // err_overflow
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            #ifndef ARDUINO_ARCH_AVR
                Cstring operator = (const struct tm st) {                       // for assigning struct tm to Cstring, like: a = st;
                    char buf [80];
                    #ifdef __LOCALE_HPP__
                        strftime (buf, sizeof (buf), lc_time_locale->getTimeFormat (), &st);
                    #else
                        strftime (buf, sizeof (buf), "%Y/%m/%d %T", &st);
                    #endif

                    strncpy (this->__c_str__, buf, N + 1);
                    if (this->__c_str__ [N]) {
                        __errorFlags__ = err_overflow;                          // err_overflow
                        this->__c_str__ [N] = 0;                                // mark the end of the string regardles OVERFLOW
                    }

                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ = err_overflow;                    // err_overflow
                        this->__c_str__ [N] = 0;                                // mark the end of the string regardles OVERFLOW
                    } else {
                        this->__errorFlags__ = 0;                               // clear error after new assignment
                    }
                    return *this;
                }
            #endif


            // += operator
            Cstring& operator += (const char *other) {          // concatenate C string to Cstring, like: a += "abc";
                if (other && !(__errorFlags__ & err_overflow)) {              // check if NULL char * pointer to overcome from possible programmer's errors and that overwlow flag has not been set yet
                    strncat (this->__c_str__, other, N + 1 - strlen (this->__c_str__));
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                        this->__rTrimUnfinishedUtf8Character__ ();
                    } 
                }
                return *this;
            }

            Cstring& operator += (const Cstring& other) {      // concatenate one Cstring to anoterh, like: a += b;
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    strncat (this->__c_str__, other.__c_str__, N + 1 - strlen (this->__c_str__));
                    this->__errorFlags__ |= other.__errorFlags__;             // add all errors from other string
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                        this->__rTrimUnfinishedUtf8Character__ ();
                    } 
                }
                return *this;
            }

            template<size_t M>
            Cstring& operator += (const Cstring<M>& other) {      // concatenate one Cstring to another of different size, like: a += b;
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    strncat (this->__c_str__, other.c_str (), N + 1 - strlen (this->__c_str__));
                    this->__errorFlags__ |= other.errorFlags ();              // add all errors from other string
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                        this->__rTrimUnfinishedUtf8Character__ ();
                    }
                }
                return *this;
            }

            Cstring& operator += (const char& other) {          // concatenate a charactr to Cstring, like: a += 'b';
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    size_t l = strlen (this->__c_str__);
                    if (l < N) { 
                        this->__c_str__ [l] = other; 
                        this->__c_str__ [l + 1] = 0; 
                    } else {
                        __errorFlags__ |= err_overflow;                           // add err_overflow flag to possibe already existing error flags
                    }
                }
                return *this;
            }

            #ifdef __LOCALE_HPP__ tole še ni
                 Cstring& operator += (const utf8char& other) { // concatenate a charactr to Cstring, like d += utf8char ("🌎");
                    if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                        size_t l = strlen (this->__c_str__);
                        if ((other.c_str [l] & 0x80) == 0 && N > l) {             // 0xxxxxxx = 1 byte character
                            this->__c_str__ [l] = other.c_str [0];          
                            this->__c_str__ [l + 1] = 0; 
                        } else if ((other.c_str [0] & 0xE0) == 0xC0 && N > l + 1) {   // 110xxxxx = 2 byte character
                            this->__c_str__ [l] = other.c_str [0];          
                            this->__c_str__ [l + 1] = other.c_str [1];
                            this->__c_str__ [l + 2] = 0; 
                        } else if ((other.c_str [0] & 0xF0) == 0xE0 && N > l + 2) {   // 1110xxxx = 3 byte character
                            this->__c_str__ [l] = other.c_str [0];
                            this->__c_str__ [l + 1] = other.c_str [1];
                            this->__c_str__ [l + 2] = other.c_str [2];
                            this->__c_str__ [l + 3] = 0; 
                        } else if ((other.c_str [0] & 0xF8) == 0xF0 && N > l + 3) {   // 11110xxx = 4 byte character
                            this->__c_str__ [l] = other.c_str [0];
                            this->__c_str__ [l + 1] = other.c_str [1];
                            this->__c_str__ [l + 2] = other.c_str [2];
                            this->__c_str__ [l + 3] = other.c_str [3];
                            this->__c_str__ [l + 4] = 0; 
                        } else {                                            // overflow
                            __errorFlags__ |= err_overflow;                      // err_overflow
                        }
                    }
                    return *this;
                }
            #endif

            Cstring& operator += (const int number) {                 // concatenate an int to Cstring, like: a += 12;
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    size_t l = strlen (this->__c_str__);
                    snprintf (this->__c_str__ + l, N + 2 - l, "%i", number);
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    } 
                }
                return *this;
            }

            Cstring& operator += (const unsigned int number) {        // concatenate an int to Cstring, like: a += 12;
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    size_t l = strlen (this->__c_str__);
                    snprintf (this->__c_str__ + l, N + 2 - l, "%u", number);
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    }
                } 
                return *this;
            }   

            Cstring operator += (const long number) {                // concatenate a long to Cstring, like: a += 12;
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    size_t l = strlen (this->__c_str__);
                    snprintf (this->__c_str__ + l, N + 2 - l, "%ld", number);
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    }
                } 
                return *this;
            }   

            Cstring operator += (const unsigned long number) {        // concatenate an unsigned long to Cstring, like: a += 12;
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    size_t l = strlen (this->__c_str__);
                    snprintf (this->__c_str__ + l, N + 2 - l, "%lu", number);
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    }
                } 
                return *this;
            }   

            // this doesn't work on AVR boards
            #ifndef ARDUINO_ARCH_AVR
                Cstring operator += (const long long number) {           // concatenate a long to Cstring, like: a += 12;
                    if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                        size_t l = strlen (this->__c_str__);
                        snprintf (this->__c_str__ + l, N + 2 - l, "%lld", number);
                        if (this->__c_str__ [N]) {
                            this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                            this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                        }
                    } 
                    return *this;
                }   

                Cstring operator += (const unsigned long long number) {   // concatenate an unsigned long to Cstring, like: a += 12;
                    if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                        size_t l = strlen (this->__c_str__);
                        snprintf (this->__c_str__ + l, N + 2 - l, "%llu", number);
                        if (this->__c_str__ [N]) {
                            this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                            this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                        }
                    } 
                    return *this;
                }   
            #endif

            Cstring operator += (const float number) {                // concatenate a flaot to Cstring, like: a += 12;
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    size_t l = strlen (this->__c_str__);

                    #ifdef ARDUINO_ARCH_AVR
                        dtostrf (number, -(N + 1 - l), 2, this->__c_str__ + l);
                    #else
                        snprintf (this->__c_str__ + l, N + 2 - l, "%f", number);
                    #endif
                    #ifdef __LOCALE_HPP__
                        // replace decimal and thousand separators with locale settings
                        for (int i = 0; this->__c_str__ [i]; i++) {
                            if (this->__c_str__ [i] == '.')
                                this->__c_str__ [i] = lc_numeric_locale->getDecimalSeparator ();
                            else if (this->__c_str__ [i] == ',')
                                this->__c_str__ [i] = lc_numeric_locale->getThousandsSeparator ();
                        }
                    #endif                
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    }
                } 
                return *this;
            }   

            Cstring operator += (const double number) {                // concatenate a double to Cstring, like: a += 12;
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    size_t l = strlen (this->__c_str__);

                    #ifdef ARDUINO_ARCH_AVR
                        dtostrf (number, -(N + 1 - l), 2, this->__c_str__ + l);
                    #else
                        snprintf (this->__c_str__ + l, N + 2 - l, "%lf", number);
                    #endif
                    #ifdef __LOCALE_HPP__
                        // replace decimal and thousand separators with locale settings
                        for (int i = 0; this->__c_str__ [i]; i++) {
                            if (this->__c_str__ [i] == '.')
                                this->__c_str__ [i] = lc_numeric_locale->getDecimalSeparator ();
                            else if (this->__c_str__ [i] == ',')
                                this->__c_str__ [i] = lc_numeric_locale->getThousandsSeparator ();
                        }
                    #endif                
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    }
                } 
                return *this;
            }   


            Cstring operator += (const long double number) {           // concatenate a double to Cstring, like: a += 12;
                if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                    size_t l = strlen (this->__c_str__);

                    #ifdef ARDUINO_ARCH_AVR
                        dtostrf (number, -(N + 1 - l), 2, this->__c_str__ + l);
                    #else
                        snprintf (this->__c_str__ + l, N + 2 - l, "%llf", number);
                    #endif
                    #ifdef __LOCALE_HPP__
                        // replace decimal and thousand separators with locale settings
                        for (int i = 0; this->__c_str__ [i]; i++) {
                            if (this->__c_str__ [i] == '.')
                                this->__c_str__ [i] = lc_numeric_locale->getDecimalSeparator ();
                            else if (this->__c_str__ [i] == ',')
                                this->__c_str__ [i] = lc_numeric_locale->getThousandsSeparator ();
                        }
                    #endif                
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    }
                } 
                return *this;
            }   

            #ifndef ARDUINO_ARCH_AVR
                Cstring operator += (const struct tm st) {                    // concatenate a struct tm to Cstring, like: a += st;
                    if (!(__errorFlags__ & err_overflow)) {                       // if overwlow flag has not been set yet
                        size_t l = strlen (this->__c_str__);
                        char buf [80];
                        #ifdef __LOCALE_HPP__
                            strftime (buf, sizeof (buf), lc_time_locale->getTimeFormat (), &st);
                        #else
                            strftime (buf, sizeof (buf), "%Y/%m/%d %T", &st);
                        #endif

                        strncat (this->__c_str__, buf, N + 1 - strlen (this->__c_str__));
                        if (this->__c_str__ [N]) {
                            this->__errorFlags__ |= err_overflow;                 // add err_overflow flag to possibe already existing error flags
                            this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                            this->__rTrimUnfinishedUtf8Character__ ();
                        } 
                    } 
                    return *this;
                }
            #endif   


            // + operator
            Cstring operator + (const char *other) const {             // for adding C string to Cstring, like: a + "abc";
                Cstring<N> tmp = *this; // copy of this, including error information
                tmp += other;
                return tmp;
            }
        
            Cstring operator + (const Cstring& other) const {       // for concatenating one Cstring with anoterh, like: a + b;
                Cstring<N> tmp = *this; // copy of this, including error information
                tmp += other;
                return tmp;
            }

            template<size_t M>
            Cstring operator + (const Cstring<M>& other) const {          // concatenate one Cstring to another of different size, like: a + b;
                Cstring<N> tmp = *this; // copy of this, including error information
                tmp += other;
                return tmp;
            }

            Cstring operator + (const char& other) const {           // for adding a character to Cstring, like: a + 'b';
                Cstring<N> tmp = *this; // copy of this, including error information
                tmp += other;
                return tmp;
            } 

            // can't use + operator for integers, this would make impossible to use for example Cstring + int to calculate the pointer to the location 

        
            // logical operators: ==, !=, <, <=, >, >=, ignore all possible errors

            inline bool operator == (const char *other) const __attribute__((always_inline))        { return !strcmp (this->__c_str__, other); }              // Cstring : C string   
            inline bool operator == (const Cstring& other) const __attribute__((always_inline))     { return !strcmp (this->__c_str__, other.__c_str__); }    // Cstring : Cstring
            template<size_t M>
            inline __attribute__((always_inline))
            bool operator == (const Cstring<M>& other) const { return !strcmp (this->__c_str__, other.c_str ()); }    // Cstring : Cstring

            inline bool operator != (const char *other) const __attribute__((always_inline))        { return strcmp (this->__c_str__, other); }               // Cstring : C string
            inline bool operator != (const Cstring& other) const __attribute__((always_inline))     { return strcmp (this->__c_str__, other.__c_str__); }     // Cstring : Cstring
            template<size_t M>
            inline __attribute__((always_inline))
            bool operator != (const Cstring<M>& other) const { return strcmp (this->__c_str__, other.c_str ()); }    // Cstring : Cstring

            #ifndef __LOCALE_HPP__
                inline bool operator <  (const char *other) const __attribute__((always_inline))        { return strcmp (this->__c_str__, other) < 0; }           // Cstring : C string
                inline bool operator <  (const Cstring& other) const __attribute__((always_inline))     { return strcmp (this->__c_str__, other.__c_str__) < 0; } // Cstring : Cstring
                template<size_t M>
                inline __attribute__((always_inline))
                bool operator < (const Cstring<M>& other) const { return strcmp (this->__c_str__, other.c_str ()) < 0; }    // Cstring : Cstring

                inline bool operator <= (const char *other) const __attribute__((always_inline))        { return strcmp (this->__c_str__, other) <= 0; }          // Cstring : C string
                inline bool operator <= (const Cstring& other) const __attribute__((always_inline))     { return strcmp (this->__c_str__, other.__c_str__) <= 0; }// Cstring : Cstring
                template<size_t M>
                inline __attribute__((always_inline))
                bool operator <= (const Cstring<M>& other) const { return strcmp (this->__c_str__, other.c_str ()) <= 0; }    // Cstring : Cstring

                inline bool operator >  (const char *other) const __attribute__((always_inline))        { return strcmp (this->__c_str__, other) > 0; }           // Cstring : C string    
                inline bool operator >  (const Cstring& other) const __attribute__((always_inline))     { return strcmp (this->__c_str__, other.__c_str__) > 0; } // Cstring : Cstring
                template<size_t M>
                inline __attribute__((always_inline))
                bool operator > (const Cstring<M>& other) const { return strcmp (this->__c_str__, other.c_str ()) > 0; }    // Cstring : Cstring

                inline bool operator >= (const char *other) const __attribute__((always_inline))        { return strcmp (this->__c_str__, other) >= 0; }          // Cstring : C string    
                inline bool operator >= (const Cstring& other) const __attribute__((always_inline))     { return strcmp (this->__c_str__, other.__c_str__) >= 0; }// Cstring : Cstring
                template<size_t M>
                inline __attribute__((always_inline))
                bool operator >= (const Cstring<M>& other) const { return strcmp (this->__c_str__, other.c_str ()) >= 0; }    // Cstring : Cstring

            #else
                inline bool operator <  (const char *other) const __attribute__((always_inline))        { return strcoll (this->__c_str__, other) < 0; }           // Cstring : C string
                inline bool operator <  (const Cstring& other) const __attribute__((always_inline))     { return strcoll (this->__c_str__, other.__c_str__) < 0; } // Cstring : Cstring
                template<size_t M>
                inline __attribute__((always_inline))
                bool operator < (const Cstring<M>& other) const { return strcoll (this->__c_str__, other.c_str ()) < 0; }    // Cstring : Cstring

                inline bool operator <= (const char *other) const __attribute__((always_inline))        { return strcoll (this->__c_str__, other) <= 0; }          // Cstring : C string
                inline bool operator <= (const Cstring& other) const __attribute__((always_inline))     { return strcoll (this->__c_str__, other.__c_str__) <= 0; }// Cstring : Cstring
                template<size_t M>
                inline __attribute__((always_inline))
                bool operator <= (const Cstring<M>& other) const { return strcoll (this->__c_str__, other.c_str ()) <= 0; }    // Cstring : Cstring

                inline bool operator >  (const char *other) const __attribute__((always_inline))        { return strcoll (this->__c_str__, other) > 0; }           // Cstring : C string    
                inline bool operator >  (const Cstring& other) const __attribute__((always_inline))     { return strcoll (this->__c_str__, other.__c_str__) > 0; } // Cstring : Cstring
                template<size_t M>
                inline __attribute__((always_inline))
                bool operator > (const Cstring<M>& other) const { return strcoll (this->__c_str__, other.c_str ()) > 0; }    // Cstring : Cstring

                inline bool operator >= (const char *other) const __attribute__((always_inline))        { return strcoll (this->__c_str__, other) >= 0; }          // Cstring : C string    
                inline bool operator >= (const Cstring& other) const __attribute__((always_inline))     { return strcoll (this->__c_str__, other.__c_str__) >= 0; }// Cstring : Cstring
                template<size_t M>
                inline __attribute__((always_inline))
                bool operator >= (const Cstring<M>& other) const { return strcoll (this->__c_str__, other.c_str ()) >= 0; }    // Cstring : Cstring

            #endif


            // [] operator
            // inline char &operator [] (size_t i) __attribute__((always_inline)) { return __c_str__ [i]; }
            // inline const char &operator [] (size_t i) const __attribute__((always_inline)) { return __c_str__ [i]; }


            // some std::string-like member functions
            inline char *c_str () __attribute__((always_inline)) { return __c_str__; } 
            inline const char *c_str () const __attribute__((always_inline)) { return (const char *) __c_str__; } 
        
            inline size_t length () __attribute__((always_inline)) { return strlen (__c_str__); } 

            size_t characters () { // returns the number of characters in the string, considering UTF-8 encoding
                // count UTF-8 characters, the first UTF-8 byte starts with 0xxxxxxx for ASCII, 110xxxxx for 2 byte character, 1110xxxx for 3 byte character and 11110xxx for 4 byte character, all the following bytes start with 10xxxxxx
                size_t count = 0;
                for (size_t i = 0; __c_str__ [i]; ) {
                    char c = __c_str__ [i];
                    if ((c & 0x80) == 0) // 1-byte character
                        i += 1;
                    else if ((c & 0xE0) == 0xC0) // 2-byte character
                        i += 2;
                    else if ((c & 0xF0) == 0xE0) // 3-byte character
                        i += 3;
                    else if ((c & 0xF8) == 0xF0) // 4-byte character
                        i += 4;
                    else // invalid UTF-8 character
                        i += 1;

                    count ++;
                }
                return count;
            }
            
            inline size_t max_size () __attribute__((always_inline)) { return N; } 

            Cstring<N> substr (size_t pos = 0, size_t len = N + 1) {
                Cstring<N> r;
                r.__errorFlags__ = this->__errorFlags__;                      // inherit all errors from original string
                if (pos >= strlen (this->__c_str__)) {
                    r.__errorFlags__ |= err_out_of_range;
                } else {
                    strncpy (r.__c_str__, this->__c_str__ + pos, len);        // can't err_overflow 
                }
                return r;
            }

            size_t find (const char *str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str);
                if (p)  return p - __c_str__;
                return npos;
            }

            size_t find (Cstring str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str.__c_str__);
                if (p)  return p - __c_str__;
                return npos;
            }

            size_t rfind (char *str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str);
                char *q = NULL;
                while (p) { q = p; p = strstr (p + 1, str); }
                if (q) return q - __c_str__;
                return npos;
            }            

            size_t rfind (Cstring str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str.__c_str__);
                char *q = NULL;
                while (p) { q = p; p = strstr (p + 1, str); }
                if (q) return q - __c_str__;
                return npos;
            }            

            void erase (size_t pos = 0) {
                if (pos > N) pos = N;
                __c_str__ [pos] = 0;
            }
        
            // some Arduino String-like member functions
            Cstring substring (size_t from = 0, size_t to = N - 1) {
                Cstring<N> r;
                r.__errorFlags__ = this->__errorFlags__;                      // inherit all errors from original string
                if (from >= strlen (this->__c_str__) || to < from) {
                    r.__errorFlags__ |= err_out_of_range;
                } else {
                    strncpy (r.__c_str__, this->__c_str__ + from, to - from); // can't err_overflow 
                }
                return r;
            }

            int indexOf (const char *str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str);
                if (p)  return p - __c_str__;
                return -1;
            }

            int indexOf (Cstring str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str.__c_str__);
                if (p)  return p - __c_str__;
                return -1;
            }

            int lastIndexOf (char *str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str);
                char *q = NULL;
                while (p) { q = p; p = strstr (p + 1, str); }
                if (q) return q - __c_str__;
                return -1;
            }

            int lastIndexOf (Cstring str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str.__c_str__);
                char *q = NULL;
                while (p) { q = p; p = strstr (p + 1, str); }
                if (q) return q - __c_str__;
                return -1;
            }

            bool endsWith (const char *str) { 
              size_t lStr = strlen (str);
              size_t lThis = strlen (__c_str__);
              if (lStr > lThis) return false;
              return !strcmp (str, __c_str__ + (lThis - lStr));
            }

            void remove (size_t pos = 0) {
                if (pos > N) pos = N;
                this->__rTrimUnfinishedUtf8Character__ ();
                // __c_str__ [pos] = 0;
            }

            void trim () {
                lTrim ();
                rTrim ();
            }
        
        
            // add other functions that may be useful 
            void toupper () {
                #ifndef __LOCALE_HPP__ // use ASCII codes
                    char *pc = __c_str__;
                    while (*pc)
                        if (*pc >= 'a' && *pc <= 'z')
                            *pc = (*pc - ('a' - 'A'));
                #else // use locale, due to UTF-8 encodint the result string may (theoretically) be longer than the original one
                    ::toupper (__c_str__);
                #endif
            }

            void tolower () {
                #ifndef __LOCALE_HPP__ // use ASCII codes
                    char *pc = __c_str__;
                    while (*pc)
                        if (*pc >= 'A' && *pc <= 'Z')
                            *pc = (*pc + ('a' - 'A'));
                #else // use locale, due to UTF-8 encodint the result string may (theoretically) be longer than the original one
                    ::tolower (__c_str__);
                #endif
            }

            void lTrim () {
                size_t i = 0;
                while (__c_str__ [i ++] == ' ');
                if (i) strcpy (__c_str__, __c_str__ + i - 1);
            }    
            
            void rTrim () {
                size_t i = strlen (__c_str__);
                while (__c_str__ [-- i] == ' ');
                __c_str__ [i + 1] = 0;
            }

            void rPad (size_t toLength, char withChar) {
                if (toLength > N) {
                  toLength = N;
                  __errorFlags__ |= err_overflow;                                 // error? overflow?
                }
                size_t l = strlen (__c_str__);
                while (l < toLength) __c_str__ [l ++] = withChar;
                __c_str__ [l] = 0;
            }

            bool isValidUtf8 () { 
                size_t i = 0;
                for (int i = 0; __c_str__ [i]; i ++) {
                    unsigned char c = __c_str__ [i];

                    if (c <= 0x7F) { 
                        // ASCII character (1-byte sequence)
                    } else if ((c & 0xE0) == 0xC0) { 
                        // 2-byte sequence
                        i ++;
                        if (__c_str__ [i] & 0xC0 != 0x80)
                            return false; // invalid continuation byte
                    } else if ((c & 0xF0) == 0xE0) { 
                        // 3-byte sequence
                        i ++;
                        if (__c_str__ [i] & 0xC0 != 0x80)
                            return false; // invalid continuation byte
                        i ++;
                        if (__c_str__ [i] & 0xC0 != 0x80)
                            return false; // invalid continuation bytes
                    } else if ((c & 0xF8) == 0xF0) { 
                        // 4-byte sequence
                        i ++;
                        if (__c_str__ [i] & 0xC0 != 0x80)
                            return false; // invalid continuation byte
                        i ++;
                        if (__c_str__ [i] & 0xC0 != 0x80)
                            return false; // invalid continuation byte
                        i ++;
                        if (__c_str__ [i] & 0xC0 != 0x80)
                            return false; // invalid continuation bytes
                    } else {
                        return false; // invalid leading byte
                    }
                }
                return true; // all characters are valid UTF-8
            }

        private:

            void __rTrimUnfinishedUtf8Character__ () {                            // trim (unfinished) UTF-8 byte from the right
                // the first UTF-8 byte starts with 0xxxxxxx for ASCII, 110xxxxx for 2 byte character, 1110xxxx for 3 byte character and 11110xxx for 4 byte character, all the following bytes start with 10xxxxxx
                int i;
                for (i = N - 1; i >= 0; i --) {
                    if ((__c_str__ [i] & 0b11000000) == 0b10000000) {
                        __c_str__ [i] = 0; // clear UTF-8 byte 
                    } else {
                        break;
                    }
                }
                if (i >= 0 && (__c_str__ [i] & 0b11000000) == 0b11000000) {
                    __c_str__ [i] = 0; // clear the first UTF-8 byte as well
                }
            }

    };

#endif
