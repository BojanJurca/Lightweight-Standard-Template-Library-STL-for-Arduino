/*
 *  locale.hpp for Arduino
 *
 *  This file is part of cin, cout library for Arduino: https://github.com/BojanJurca/cin-cout-for-Arduino
 *
 *  March 12, 2026, Bojan Jurca
 *
 */


#ifndef __LOCALE_HPP__
    #define __LOCALE_HPP__

    struct utf8char {
        char c_str [5] = {}; // max 4 + null terminator
        public:
        utf8char () {}

        inline utf8char (const char* s) {
            int len = length (static_cast<unsigned char> (s [0]));
            memcpy (c_str, s, len);
            c_str [len] = '\0';
        }

        inline bool operator == (const utf8char& other) const {
            return strcmp (c_str, other.c_str) == 0;
        }

        inline int length (unsigned char lead) {
            if ((lead & 0x80) == 0) return 1;           // 0xxxxxxx
            if ((lead & 0xE0) == 0xC0) return 2;        // 110xxxxx
            if ((lead & 0xF0) == 0xE0) return 3;        // 1110xxxx
            if ((lead & 0xF8) == 0xF0) return 4;        // 11110xxx
            return 0; // doesn't happen
        }
    };

    class utf8_iterator {
        const char* __ptr__;

    public:
        utf8_iterator (const char* ptr) : __ptr__ (ptr) {}

            static int length (unsigned char lead) {
                if ((lead & 0x80) == 0) return 1;           // 0xxxxxxx
                if ((lead & 0xE0) == 0xC0) return 2;        // 110xxxxx
                if ((lead & 0xF0) == 0xE0) return 3;        // 1110xxxx
                if ((lead & 0xF8) == 0xF0) return 4;        // 11110xxx
                return 0; // doesn't happen
            }

        inline utf8char operator *() const {
            int len = length (static_cast<unsigned char> (*__ptr__));
            utf8char u8c;
            memcpy (u8c.c_str, __ptr__, len);
            u8c.c_str [len] = '\0';
            return u8c;
        }

        inline char* get () const { return (char *) __ptr__; }

        inline void set (const utf8char& c) {
            int len = length (static_cast<unsigned char> (*__ptr__));
            memcpy ((void *) __ptr__, c.c_str, len); // overwrite in-place
        }

        inline utf8_iterator& operator ++ () {
            int len = length (static_cast<unsigned char> (*__ptr__));
            __ptr__ += len;
            return *this;
        }

        inline bool operator < (const utf8_iterator& other) const { return __ptr__ < other.__ptr__; }
        inline bool operator <= (const utf8_iterator& other) const { return __ptr__ <= other.__ptr__; }
    };


    // Please note that not all locale categories are supported
    enum localeCategory_t {
        lc_collate  = 0b00000001, // LC_COLLATE is used for string comparison and sorting. It defines how strings are compared and sorted according to the rules of the specified locale.
        lc_ctype    = 0b00000010, // LC_CTYPE is crucial for character classification and conversion functions. It governs what’s considered a letter, digit, punctuation mark, etc., and how characters convert between uppercase and lowercase.
        // lc_monetary = 0b00000100, // LC_MONETARY is used to format monetary values according to the rules of a specified locale.
        lc_numeric  = 0b00001000, // LC_NUMERIC controls the formatting of numbers, specifically the decimal point and thousands separator, according to the rules of a specified locale.
        lc_time     = 0b00010000, // LC_TIME controls the formatting of dates and times according to the locale's rules.
        // lc_messages = 0b00100000, // LC_MESSAGES handles the localization of system messages and prompts. It ensures that messages, warnings, and errors are displayed in the appropriate language and format for the user's locale.
        lc_all      = 0b00111111
    };


    // ----- The default locale - ASCII locale, which is also a base class for all other locales -----

    class locale {
        public:
            // list of supported locales
            locale *nextLocale = NULL;

            // locale name
            virtual inline const char* name () const { return "ASCII"; }

            // lc_collate
            virtual inline int strcoll (const char *s1, const char *s2) {
                return strcmp (s1, s2);
            }

            // lc_ctype
            virtual inline bool toupper (char *ps) {
                while (*ps) {
                    if (*ps >= 'a' && *ps <= 'z')
                        *ps = (*ps - ('a' - 'A'));
                    ps ++;
                }
                return true;
            }

            virtual inline bool tolower (char *ps) {
                while (*ps) {
                    if (*ps >= 'A' && *ps <= 'Z')
                        *ps = (*ps + ('a' - 'A'));
                    ps ++;
                }
                return true;
            }            

            // lc_numeric
            virtual inline char getDecimalSeparator () const { return '.'; }
            virtual inline char getThousandsSeparator () const { return ','; }
            // lc_time
            virtual inline const char* getTimeFormat () const { return "%Y/%m/%d %r"; }
    };

    // Create a Meyers Singleton working instances
    inline locale& __lc_default__ () {
        static locale instance;
        return instance;
    }

    // ----- Locale en_150.UTF-8  -----
    class en_150_UTF_8_locale : public locale {
        public:
            // locale name
            inline const char* name () const override { return "en_150.UTF-8"; }
            // lc_ctype
            // lc_numeric
            inline char getDecimalSeparator () const override { return ','; }
            inline char getThousandsSeparator () const override { return '.'; }
            // lc_time
            inline const char* getTimeFormat () const override { return "%d/%m/%Y %H:%M:%S"; }
    };

    // Add new locale instance to the supported locale list
    inline bool addlocale (locale *loc) {
        if (loc == NULL || loc->name () == NULL) // only the default locale has no ID string
            return false;

        locale *p = &__lc_default__ ();
        while (p->nextLocale != NULL) {
            if (!strcmp (p->name (), loc->name ()))
                return false;
            p = p->nextLocale;
        }

        p->nextLocale = loc;
        return true;
    };

    // Create a instance and insert it into supported locale list
    #ifdef ARDUINO_ARCH_AVR
        bool __locale_en_150_UTF_8__ = addlocale (new en_150_UTF_8_locale);
    #else
        bool __locale_en_150_UTF_8__ = addlocale (new (std::nothrow) en_150_UTF_8_locale);
    #endif

    // setlocale
    // inline locale *lc_collate_locale = &default_locale;
    inline locale*& __lc_collate_locale_ptr__ () {
        static locale* ptr = &__lc_default__ ();
        return ptr;
    }
    #define lc_collate_locale (__lc_collate_locale_ptr__ ())
    
    // Meyers Singletons with indirection

    // inline locale *lc_ctype_locale = &default_locale;
    inline locale*& __lc_ctype_locale_ptr__ () {
        static locale* ptr = &__lc_default__ ();
        return ptr;
    }
    #define lc_ctype_locale (__lc_ctype_locale_ptr__ ())

    // inline locale *lc_numeric_locale = &default_locale;
    inline locale*& __lc_numeric_locale_ptr__ () {
        static locale* ptr = &__lc_default__ ();
        return ptr;
    }
    #define lc_numeric_locale (__lc_numeric_locale_ptr__ ())

    // inline locale *lc_time_locale = &default_locale;
    inline locale*& __lc_time_locale_ptr__ () {
        static locale* ptr = &__lc_default__ ();
        return ptr;
    }
    #define lc_time_locale (__lc_time_locale_ptr__ ())        

    inline bool setlocale (localeCategory_t category, const char *name) {
        // find locale by name
        locale *p = &__lc_default__ ();
        while (p && strcmp (p->name (), name))
            p = p->nextLocale;

        if (!p) // not found
            return false;

        #ifndef ARDUINO_ARCH_AVR
            if (category & lc_time)
                // lc_time_locale = p;
                __lc_time_locale_ptr__ () = p; // rebind
        #else
            if (category & lc_time)
                return false; // AVR boards do not support struct tm
        #endif

        if (category & lc_collate)
            // lc_collate_locale = p;
            __lc_collate_locale_ptr__ () = p; // rebind

        if (category & lc_ctype)
            // lc_ctype_locale = p;
            __lc_ctype_locale_ptr__ () = p; // rebind

        if (category & lc_numeric)
            // lc_numeric_locale = p;
            __lc_numeric_locale_ptr__ () = p; // rebind

        return true;
    }

    // strcoll
    inline int strcoll (const char *s1, const char *s2) { return lc_collate_locale->strcoll (s1, s2); } 
    inline int strcoll (String& s1, String& s2) { return lc_collate_locale->strcoll ((char *) s1.c_str (), (char *) s2.c_str ()); } 

    // toupper, tolower
    inline bool toupper (char *cp) { return lc_ctype_locale->toupper (cp); }
    inline bool toupper (String& s) { return lc_ctype_locale->toupper ((char *) s.c_str ()); }
    inline bool tolower (char *cp) { return lc_ctype_locale->tolower (cp); }
    inline bool tolower (String& s) { return lc_ctype_locale->tolower ((char *) s.c_str ()); }

#endif
