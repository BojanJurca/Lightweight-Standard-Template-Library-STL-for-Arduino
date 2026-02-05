/*
 *  agorithm.hpp for Arduino
 *
 *  This file is part of Lightweight C++ Standard Template Library (STL) for Arduino: https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino
 *
 *  February 6, 2026, Bojan Jurca, with great help of Microsoft Copilot regarding templates 
 *
 */


#ifndef __ALGORITHM_HPP__
    #define __ALGORITHM_HPP__

        namespace algorithm {

            template<typename T1, typename T2> bool equals (const T1& a, const T2& b) { return a == b; }
            template<> bool equals (const char* const& a, const char* const& b) { return strcmp (a, b) == 0; }
            template<> bool equals (char* const& a, char* const& b) { return strcmp (a, b) == 0; }
            template<> bool equals (const char* const& a, char* const& b) { return strcmp (a, b) == 0; }
            template<> bool equals (char* const& a, const char* const& b) { return strcmp (a, b) == 0; }

            #ifndef __LOCALE_HPP__s
                template<typename T1, typename T2> bool smaller (const T1& a, const T2& b) { return a < b; }
                template<> bool smaller (const char* const& a, const char* const& b) { return strcmp (a, b) < 0; }
                template<> bool smaller (char* const& a, char* const& b) { return strcmp (a, b) < 0; }
                template<> bool smaller (const char* const& a, char* const& b) { return strcmp (a, b) < 0; }
                template<> bool smaller (char* const& a, const char* const& b) { return strcmp (a, b) < 0; }

                template<typename T1, typename T2> bool greater (const T1& a, const T2& b) { return a > b; }
                template<> bool greater (const char* const& a, const char* const& b) { return strcmp (a, b) > 0; }
                template<> bool greater (char* const& a, char* const& b) { return strcmp (a, b) > 0; }
                template<> bool greater (const char* const& a, char* const& b) { return strcmp (a, b) > 0; }
                template<> bool greater (char* const& a, const char* const& b) { return strcmp (a, b) > 0; }
            #else
                template<typename T1, typename T2> bool smaller (const T1& a, const T2& b) { return a < b; }
                template<> bool smaller (const char* const& a, const char* const& b) { return strcoll (a, b) < 0; }
                template<> bool smaller (char* const& a, char* const& b) { return strcoll (a, b) < 0; }
                template<> bool smaller (const char* const& a, char* const& b) { return strcoll (a, b) < 0; }
                template<> bool smaller (char* const& a, const char* const& b) { return strcoll (a, b) < 0; }

                template<typename T1, typename T2> bool greater (const T1& a, const T2& b) { return a > b; }
                template<> bool greater (const char* const& a, const char* const& b) { return strcoll (a, b) > 0; }
                template<> bool greater (char* const& a, char* const& b) { return strcoll (a, b) > 0; }
                template<> bool greater (const char* const& a, char* const& b) { return strcoll (a, b) > 0; }
                template<> bool greater (char* const& a, const char* const& b) { return strcoll (a, b) > 0; }
            #endif

        };


       /*
        *   find element in [first, last) iterators, using forward iterator
        */

        template <typename forwardIterator, typename T>
        forwardIterator find (forwardIterator first, forwardIterator last, const T& value) {
            while (first != last) {
              if (algorithm::equals (*first, value)) 
                  return first;
              ++ first;
            }
            return last; // not found
        }



       /*
        *   find min element in [first, last) iterators, using forward iterator
        */

        template <typename forwardIterator>
        forwardIterator min_element (forwardIterator first, forwardIterator last) {
            forwardIterator m = first;

            while (first != last) {
                if (algorithm::smaller (*first, *m)) 
                    m = first;
                ++ first;
            }

            return m;
        }



       /*
        *   find max element in [first, last) iterators, using forward iterator
        */

        template <typename forwardIterator>
        forwardIterator max_element (forwardIterator first, forwardIterator last) {
            forwardIterator m = first;

            while (first != last) {
                if (algorithm::greater (*first, *m)) 
                    m = first;
                ++ first;
            }

            return m;
        }


       /*
        *   general sort template:
        *
        *     - create our own remove_reference since AVR boards do not support std::remove_reference
        *     - chreate sortHelper to distinguish between lists and vectors/arrays with the help of remove_reference
        *     - create sort function that uses sortHelper
        */

        // Primary remove_reference template
        template <typename T>
        struct remove_reference {
            using type = T;
        };

        // Partial remove_reference specialization for lvalue references
        template <typename T>
        struct remove_reference<T&> {
            using type = T;
        };

        // Partial specialization for rvalue references
        template <typename T>
        struct remove_reference<T&&> {
            using type = T;
        };

        // Helper alias template to simplify usage
        template <typename T>
        using remove_reference_t = typename remove_reference<T>::type;


        // General sort function
        template <typename T, typename U> 
        void sort (T first, U last);


       /*
        *   (merge)sort list elements in [first, last) iterators, using forward iterator
        */


    #ifdef __LIST_HPP__
        template <typename forwardIterator>
        void __mergeSort__ (forwardIterator first, forwardIterator last) {
            // check if number of elements is <= 1 so sorting isn't necessary
            if (first.__p__ == last.__p__ || first.__p__->next == last.__p__)
                return;
            // form here on there are at least 2 elements in the list - this is inportant, since lists are only single linked so we can't change the pointer to the first element the same way as we can change the other pointers

            using listElementType = typename remove_reference<decltype(*first)>::type; 
            list<listElementType> workingList [4];
            list<listElementType> *pPrimaryOutputList = &workingList [0];
            list<listElementType> *pStandbyOutputList = &workingList [1];
            list<listElementType> *pFirstInputList = &workingList [2];
            list<listElementType> *pSecondInputList = &workingList [3];

            // do the first merging from first iterator to workingList [1] and workingList [0] and loacate min element meanwhile
            auto pMin = first.__p__;
            while (first.__p__->next != last.__p__) {
                // 1. swith output lists if needed
                if (pStandbyOutputList->__front__ && algorithm::smaller (first.__p__->next->element, pStandbyOutputList->back ())) {
                    list<listElementType> *pTmpOutputList = pStandbyOutputList;
                    pStandbyOutputList = pPrimaryOutputList;
                    pPrimaryOutputList = pTmpOutputList;
                }
                // 2. is this min element so far?
                if (algorithm::smaller (first.__p__->next->element, pMin->element))
                    pMin = first.__p__->next;
                // 3. add first.__p__->next to the back of pStandbyOutputListaryOutputList and remove it from p list
                if (pStandbyOutputList->__front__ == NULL)
                    pStandbyOutputList->__front__ = first.__p__->next;
                if (pStandbyOutputList->__back__ == NULL)
                    pStandbyOutputList->__back__ = first.__p__->next;
                else {
                    pStandbyOutputList->__back__->next = first.__p__->next;
                    pStandbyOutputList->__back__ = pStandbyOutputList->__back__->next;
                }
                first.__p__->next = first.__p__->next->next;
                pStandbyOutputList->__back__->next = NULL;
            }

            // swith first iterator element with min element so this part of list is sorted and we won't have to deal with it again
            if (algorithm::smaller (pMin->element, first.__p__->element)) {
                // swithc elements in the way if they are objects their constructors and destructors would not get called
                //listElementType tmp;
                char tmp [sizeof (listElementType)];
                memcpy (tmp, &(first.__p__->element), sizeof (listElementType));
                memcpy (&(first.__p__->element), &(pMin->element), sizeof (listElementType));
                memcpy (&(pMin->element), tmp, sizeof (listElementType));
            }

            // if we've only got a single output list then the sorting is already done and we only have to move primaryOutputList back to front iterator
            while (pStandbyOutputList->__front__) {
                // it we'we got 2 output lists the elements are not yet in order - do the mergesort magic here in the right way, otherwise we won't get O (n log n) time complexity

                    // switch input and output lists so all the elements are in two input lists and both output lists are empty
                    list<listElementType> *pTmpList = pPrimaryOutputList;
                    pPrimaryOutputList = pFirstInputList;
                    pFirstInputList = pStandbyOutputList;
                    pStandbyOutputList = pSecondInputList;
                    pSecondInputList = pTmpList;

                    // merge the input lists into output lists
                    list<listElementType> *pInputList = NULL;
                    listElementType *lastElementMerged = NULL;

                    while (pFirstInputList->__front__ || pSecondInputList->__front__) { // while there is any input

                        if (pFirstInputList->__front__) {
                            pInputList = pFirstInputList;
                            if (pSecondInputList->__front__) { // there are both inputs available    
                                if (lastElementMerged) { // pick the smallest element from inputs that is bigger than the last element already in output (this would create longer sorted output sequences)    
                                    if (algorithm::smaller (pSecondInputList->__front__->element, pFirstInputList->__front__->element) && !algorithm::smaller (pSecondInputList->__front__->element, *lastElementMerged)) { // greater or equal
                                        pInputList = pSecondInputList;
                                    }
                                } else { // just pick a smaller of both inputs (this would create longer sorted output sequences)
                                    if (algorithm::smaller (pSecondInputList->__front__->element, pFirstInputList->__front__->element)) {
                                        pInputList = pSecondInputList;
                                    }
                                }
                            }
                            // else there is only the first input available, we don't have to do anything
                        } else { // there is only the second input available
                            pInputList = pSecondInputList;
                        }

                        // we have selected the input, now see if we have to swithc the outputs
                        if (lastElementMerged && algorithm::smaller (pInputList->__front__->element, *lastElementMerged)) {
                            pTmpList = pPrimaryOutputList;
                            pPrimaryOutputList = pStandbyOutputList;
                            pStandbyOutputList = pTmpList;
                        } 

                        // 1. insert element to the output list
                        auto pTmp = pInputList->__front__->next;
                        pInputList->__front__->next = NULL;
                        if (pPrimaryOutputList->__front__ == NULL)
                            pPrimaryOutputList->__front__ = pInputList->__front__;
                        if (pPrimaryOutputList->__back__ != NULL)
                            pPrimaryOutputList->__back__->next = pInputList->__front__;
                        pPrimaryOutputList->__back__ = pInputList->__front__;
                        // 2. remove it from pInputList list
                        pInputList->__front__ = pTmp;
                        if (pInputList->__front__ == NULL)
                            pInputList->__back__ = NULL;
                        // 3. remember the last element merged so far in this iteration
                        lastElementMerged = &(pPrimaryOutputList->__back__->element);
                    } // merging inputs into outputs

            } // while we are getting more than one output

            // move all elements from (a single, which is) primaryOutputList back to front iterator
            pPrimaryOutputList->__back__ = last.__p__;
            first.__p__->next = pPrimaryOutputList->__front__;
            pPrimaryOutputList->__front__ = NULL;
        }
    #endif


       /*
        *   (heap)sort elements in [first, last) iterators, using random iterator
        */

      template <typename randomIterator>
      void __heapSort__ (randomIterator first, randomIterator last) {
            int n = last - first;

            // build the heap (rearange array)
            for (int i = n / 2 - 1; i >= 0; i --) {

                // heapify i .. n
                int j = i;
                do {
                    int largest = j;        // initialize largest as root
                    int left = 2 * j + 1;   // left = 2 * j + 1
                    int right = 2 * j + 2;  // right = 2 * j + 2
                    
                    if (left < n && algorithm::smaller (*(first + largest), *(first + left))) largest = left;     // if left child is larger than root
                    if (right < n && algorithm::smaller (*(first + largest), *(first + right))) largest = right;  // if right child is larger than largest so far
                    
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
                    
                    if (left < n && algorithm::smaller (*(first + largest), *(first + left))) largest = left;     // if left child is larger than root
                    if (right < n && algorithm::smaller (*(first + largest), *(first + right))) largest = right;  // if right child is larger than largest so far

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
        *   (heap)sort vector<String> template specialization
        */

    #ifdef __VECTOR_HPP__
        template <>
        void __heapSort__ (vector<String>::iterator first, vector<String>::iterator last) {
            int n = last - first;

            // build the heap (rearange array)
            for (int i = n / 2 - 1; i >= 0; i --) {

                // heapify i .. n
                int j = i;
                do {
                    int largest = j;        // initialize largest as root
                    int left = 2 * j + 1;   // left = 2 * j + 1
                    int right = 2 * j + 2;  // right = 2 * j + 2

                    if (left < n && algorithm::smaller (*(first + largest), *(first + left))) largest = left;     // if left child is larger than root
                    if (right < n && algorithm::smaller (*(first + largest), *(first + right))) largest = right;  // if right child is larger than largest so far

                    if (largest != j) {     // if largest is not root
                        // swap [j] and [largest]
                        char tmp [sizeof (String)];
                        memcpy (tmp, &*(first + j), sizeof (String));
                        memcpy (&*(first + j), &*(first + largest), sizeof (String));
                        memcpy (&*(first + largest), tmp, sizeof (String));                        

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
                char tmp [sizeof (String)];
                memcpy (tmp, &*(first + 0), sizeof (String));
                memcpy (&*(first + 0), &*(first + n), sizeof (String));
                memcpy (&*(first + n), tmp, sizeof (String));                        

                // heapify the reduced heap 0 .. n
                int j = 0;
                do {
                    int largest = j;        // initialize largest as root
                    int left = 2 * j + 1;   // left = 2 * j + 1
                    int right = 2 * j + 2;  // right = 2 * j + 2

                    if (left < n && algorithm::smaller (*(first + largest), *(first + left))) largest = left;     // if left child is larger than root
                    if (right < n && algorithm::smaller (*(first + largest), *(first + right))) largest = right;  // if right child is larger than largest so far

                    if (largest != j) {     // if largest is not root
                        // swap [j] and [largest]
                        char tmp [sizeof (String)];
                        memcpy (tmp, &*(first + j), sizeof (String));
                        memcpy (&*(first + j), &*(first + largest), sizeof (String));
                        memcpy (&*(first + largest), tmp, sizeof (String));                        

                        // heapify the affected subtree in the next iteration
                        j = largest;
                    } else {
                        break;
                    }
                } while (true);            
            }        

        }
    #endif


       /*
        *   (heap)sort String template specialization for arrays of Strings
        */

        template <>
        void __heapSort__ (String *first, String *last) {
            int n = last - first;

            // build the heap (rearange array)
            for (int i = n / 2 - 1; i >= 0; i --) {

                // heapify i .. n
                int j = i;
                do {
                    int largest = j;        // initialize largest as root
                    int left = 2 * j + 1;   // left = 2 * j + 1
                    int right = 2 * j + 2;  // right = 2 * j + 2

                    if (left < n && algorithm::smaller (*(first + largest), *(first + left))) largest = left;     // if left child is larger than root
                    if (right < n && algorithm::smaller (*(first + largest), *(first + right))) largest = right;  // if right child is larger than largest so far

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

                    if (left < n && algorithm::smaller (*(first + largest), *(first + left))) largest = left;     // if left child is larger than root
                    if (right < n && algorithm::smaller (*(first + largest), *(first + right))) largest = right;  // if right child is larger than largest so far

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


        template<typename RandomIt, typename Dummy>
        struct sortHelper {
            void sort (RandomIt first, RandomIt last) {
                __heapSort__ (first, last);
            }
        };

        #ifdef __LIST_HPP__
            template<typename ListIt>
            struct sortHelper<ListIt, ListIt> {
                void sort (ListIt first, ListIt last) {
                    __mergeSort__ (first, last);
                }
            };
        #endif

        template <typename T, typename U>
        void sort (T first, U last) {
            #ifdef __LIST_HPP__
                using element_type = typename remove_reference<decltype(*first)>::type;
                using list_iterator = typename list<element_type>::iterator;
                sortHelper<T, list_iterator> helper;
            #else
                sortHelper<T, T> helper;
            #endif
            helper.sort (first, last);
        }

#endif