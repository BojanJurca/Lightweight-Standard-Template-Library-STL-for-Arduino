/*
 *  agorithm.hpp for Arduino
 *
 *  This file is part of Lightweight C++ Standard Template Library (STL) for Arduino: https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino
 *
 *  October 23, 2024, Bojan Jurca
 *
 */


#ifndef __ALGORITHM_HPP__
    #define __ALGORITHM_HPP__


       /*
        *   find element in [first, last) iterators, using forward iterator
        */

        template <typename FWDIT, typename T>
        FWDIT find (FWDIT first, FWDIT last, const T& value) {
            while (first != last) {
              if (*first == value) 
                  return first;
              ++ first;
            }
            return last; // not found
        }


       /*
        *   (heap)sort elements in [first, last) iterators, using random iterator
        */

      template <typename randomIterator>
      void sort (randomIterator first, randomIterator last) {
            int n = last - first;

            // build the heap (rearange array)
            for (int i = n / 2 - 1; i >= 0; i --) {

                // heapify i .. n
                int j = i;
                do {
                    int largest = j;        // initialize largest as root
                    int left = 2 * j + 1;   // left = 2 * j + 1
                    int right = 2 * j + 2;  // right = 2 * j + 2
                    
                    if (left < n && *(first + largest) < *(first + left)) largest = left;     // if left child is larger than root
                    if (right < n && *(first + largest) < *(first + right)) largest = right;  // if right child is larger than largest so far
                    
                    if (largest != j) {     // if largest is not root
                        // swap [j] and [largest]
                        auto tmp = *(first + j); 
                        *(first + j) = *(first + largest); 
                        *(first + largest) = tmp;

                        // heapify the affected subtree in the next iteration
                        j = largest;
                    } else {
                        break;
                    }
                } while (true);
            }

            // one by one extract an element from heap
            for (-- n; n > 0; n --) {
                // move current root to end
                auto tmp = *(first + 0); 
                *(first + 0) = *(first + n);
                *(first + n) = tmp;

                // heapify the reduced heap 0 .. n
                int j = 0;
                do {
                    int largest = j;        // initialize largest as root
                    int left = 2 * j + 1;   // left = 2 * j + 1
                    int right = 2 * j + 2;  // right = 2 * j + 2
                    
                    if (left < n && *(first + largest) < *(first + left)) largest = left;     // if left child is larger than root
                    if (right < n && *(first + largest) < *(first + right)) largest = right;  // if right child is larger than largest so far

                    if (largest != j) {     // if largest is not root
                        // swap [j] and [largest]
                        auto tmp = *(first + j); 
                        *(first + j) = *(first + largest); 
                        *(first + largest) = tmp;

                        // heapify the affected subtree in the next iteration
                        j = largest;
                    } else {
                        break;
                    }
                } while (true);            
            }        

        }



       /*
        *   (heap)sort String template specialization
        */

        template <>
        void sort (vector<String>::iterator first, vector<String>::iterator last) {
            int n = last - first;

            // build the heap (rearange array)
            for (int i = n / 2 - 1; i >= 0; i --) {

                // heapify i .. n
                int j = i;
                do {
                    int largest = j;        // initialize largest as root
                    int left = 2 * j + 1;   // left = 2 * j + 1
                    int right = 2 * j + 2;  // right = 2 * j + 2

                    if (left < n && *(first + largest) < *(first + left)) largest = left;     // if left child is larger than root
                    if (right < n && *(first + largest) < *(first + right)) largest = right;  // if right child is larger than largest so far

                    if (largest != j) {     // if largest is not root
                        // swap [j] and [largest]
                        String tmp;
                        memcpy (&tmp, &*(first + j), sizeof (String));
                        memcpy (&*(first + j), &*(first + largest), sizeof (String));
                        memcpy (&*(first + largest), &tmp, sizeof (String));                        

                        // heapify the affected subtree in the next iteration
                        j = largest;
                    } else {
                        break;
                    }
                } while (true);
            }

            // one by one extract an element from heap
            for (-- n; n > 0; n --) {
                // move current root to end
                String tmp;
                memcpy (&tmp, &*(first + 0), sizeof (String));
                memcpy (&*(first + 0), &*(first + n), sizeof (String));
                memcpy (&*(first + n), &tmp, sizeof (String));                        

                // heapify the reduced heap 0 .. n
                int j = 0;
                do {
                    int largest = j;        // initialize largest as root
                    int left = 2 * j + 1;   // left = 2 * j + 1
                    int right = 2 * j + 2;  // right = 2 * j + 2

                    if (left < n && *(first + largest) < *(first + left)) largest = left;     // if left child is larger than root
                    if (right < n && *(first + largest) < *(first + right)) largest = right;  // if right child is larger than largest so far

                    if (largest != j) {     // if largest is not root
                        // swap [j] and [largest]
                        String tmp;
                        memcpy (&tmp, &*(first + j), sizeof (String));
                        memcpy (&*(first + j), &*(first + largest), sizeof (String));
                        memcpy (&*(first + largest), &tmp, sizeof (String));                        

                        // heapify the affected subtree in the next iteration
                        j = largest;
                    } else {
                        break;
                    }
                } while (true);            
            }        

        }



       /*
        *   find min element in [first, last) iterators, using forward iterator
        */

        template <typename FWDIT>
        FWDIT min_element (FWDIT first, FWDIT last) {
            FWDIT m = first;

            while (first != last) {
                if (*first < *m) 
                    m = first;
                ++ first;
            }

            return m;
        }


       /*
        *   find max element in [first, last) iterators, using forward iterator
        */

        template <typename FWDIT>
        FWDIT max_element (FWDIT first, FWDIT last) {
            FWDIT m = first;

            while (first != last) {
                if (*first > *m) 
                    m = first;
                ++ first;
            }

            return m;
        }

#endif