/*
 *  array.hpp for Arduino
 * 
 *  This file is part of Lightweight C++ Standard Template Library (STL) for Arduino: https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino
 * 
 *  The data storage is internaly implemented as balanced binary search tree for good searching performance.
 *
 *  Map functions are not thread-safe.
 * 
 *  Nov 26, 2025, Bojan Jurca
 *  
 */


 #ifndef __ARRAY_HPP__
    #define __ARRAY_HPP__


    template<class arrayType, size_t N>
    class array {
        private:
            arrayType __data__ [N];

        public:

            array () {}

            #ifndef ARDUINO_ARCH_AVR
                   /*
                    *  Constructor of array from brace enclosed initializer list allows the following kinds of creation of vectors: 
                    *  
                    *     array<int, 3> D = { 200, 300, 400 };
                    *     array<int, 2> E ( { 500, 600 } );
                    */
            
                    array (const std::initializer_list<arrayType>& il) {
                        size_t i = 0;
                        for (auto element: il)
                            __data__ [i ++] = element;
                    }
            #endif
            // #else
                   /*
                    *  Constructor of array from C array (for AVR) boards allows the following kinds of creation of array:
                    *  
                    *     array<int, 2> E ( { 500, 600 } );
                    */

                    array (const arrayType (&a) [N]) {
                        for (size_t i = 0; i < N; ++i)
                            __data__ [i] = a [i];
                    }

            // #endif


           /*
            * Returns the number of elements in the array.
            */

            size_t size () { return N; }


           /*
            * Returns the the poitner to the elements of the array.
            */

            arrayType* data () { return __data__; }


           /*
            *  [] operator enables elements of array to be addressed by their positions (indexes) like:
            *  
            *    for (int i = 0; i < E.size (); i++)
            *      Serial.printf ("E [%i] = %i\n", i, E [i]);    
            *    
            *    or
            *    
            *     E [0] = E [1];
            *     
            *  If the index is not a valid index, the result is unpredictable
            */

            inline arrayType &operator [] (size_t ind) __attribute__((always_inline)) { 
                return __data__ [ind]; 
            }


            class iterator {
                public:
                    iterator (arrayType *p) { __p__ = p; }
                    arrayType& operator *() const { return *__p__; }
                    iterator& operator ++ () { ++ __p__; return *this; }
                    iterator& operator -- () { -- __p__; return *this; }
                    int operator - (const iterator& other) { return this->__p__ - other.__p__; }
                    iterator operator + (const int& position) { return iterator (__p__ + position); }
                    iterator operator - (const int& position) { return iterator (__p__ - position); }
                    // C++ will stop iterating when != operator returns false, this is when __p__ counts to N
                    friend bool operator != (const iterator& a, const iterator& b) { return a.__p__ != b.__p__; }
                    friend bool operator == (const iterator& a, const iterator& b) { return a.__p__ == b.__p__; }

                private:
                    arrayType *__p__;

            };

            iterator begin () { return iterator (__data__); }    // first element
            iterator end () { return iterator (__data__ + N); }  // past the last element

            #ifdef __OSTREAM_HPP__
                // print array to ostream
                friend ostream& operator << (ostream& os, array& a) {
                    bool first = true;
                    os << "[";
                    for (auto e : a) {
                        if (!first)
                            os << ",";
                        first = false;
                        os << e;
                    }
                    os << "]";
                    return os;
                }
            #endif

    };

#endif