/*
 *  iostream_commin.hpp for Arduino
 *
 *  This file is part of Lightweight C++ Standard Template Library (STL) for Arduino: https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino
 *
 *  Oct 10, 2026, Bojan Jurca
 *
 */


#ifndef __ISTREAM_COMMON_HPP__
    #define __ISTREAM_COMMON_HPP__


    // Serial initialization


    // Meyer's singleton
    inline bool& __iostreamInitialized__() {
        static bool initialized = false;
        return initialized;
    }

    #ifdef ARDUINO_ARCH_AVR 
        inline void cinit (bool waitForSerial = false, unsigned int waitAfterSerial = 100, unsigned int serialSpeed = 9600) {
            auto &initialized = __iostreamInitialized__ ();
            if (!initialized) {
                Serial.begin (serialSpeed);
                initialized = true;
                if (waitForSerial)
                    while (!Serial) 
                        delay (10);
                delay (waitAfterSerial);
            }
        }
    #else
        inline void cinit (bool waitForSerial = false, unsigned int waitAfterSerial = 100, unsigned int serialSpeed = 115200) {
            auto &initialized = __iostreamInitialized__ ();
            if (!initialized) {
                Serial.begin (serialSpeed);
                initialized = true;
                if (waitForSerial)
                    while (!Serial) 
                        delay (10);
                delay (waitAfterSerial);
            }
        }
    #endif

    inline void __checkIostreamInitialized__ () {
        auto &initialized = __iostreamInitialized__ ();
        if (!initialized)
            cinit ();
    }


#endif
