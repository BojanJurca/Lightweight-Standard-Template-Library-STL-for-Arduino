#include <ostream.hpp>      // cout instance for Arduino
#include <complex.hpp>      // complex numbers for Arduino


// Fast Fouriere Transform using Radix-2 algorithm with complexity of O (n log n), according to https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
template<typename T, size_t N>
void fft (complex<T> (&output) [N], const complex<T> (&input) [N]) {
    static_assert((N > 0) && ((N & (N - 1)) == 0), "N must be a power of 2");

    // Calculate the number of stages to perform: 2^S = N
    int S = 0;
    for (int s = 1; s < N; s *= 2)
        S++;

    // Bit-reverse copy: output <- input
    for (size_t i = 0; i < N; i++) {
        size_t iTmp = i;
        size_t iReversed = 0;
        for (int s = 1; s <= S; s++) {
            iReversed = (iReversed << 1) | (iTmp & 0x01);
            iTmp >>= 1;
        }
        output [iReversed] = input [i];
    }

    // Go through all the stages
    for (int s = 1; s <= S; s++) {
        int m = 1 << s; // m = 2^s
        complex<T> omegam = exp (complex<T> (0, -2 * M_PI / m));
        for (size_t k = 0; k < N; k += m) {
            complex<T> omega = { 1.f, 0.f };
            for (size_t j = 0; j < m / 2; j++) {
                complex<T> t = omega * output [k + j + m / 2];
                complex<T> u = output [k + j];
                output [k + j] = u + t;
                output [k + j + m / 2] = u - t;
                omega *= omegam;
            }
        }
    }
}

void setup () {

    cinit (true);                                             // three optional arguments: bool waitForSerial = false, unsigned int waitAfterSerial = 100 [ms], unsigned int serialSpeed = 115200 (9600 for AVR boards)

    #define N 256

    int signal [N] = {382,310,172,107,141,285,439,423,206,-40,-121,45,221,166,-7,-64,50,119,99,123,226,321,278,77,-123,-154,-60,-27,-67,-49,33,64,-58,-240,-304,-290,-249,-195,-157,-187,-277,-281,-129,1,35,104,245,349,351,270,178,135,127,137,192,288,360,317,206,118,54,-28,-134,-204,-215,-231,-272,-322,-300,-207,-128,-134,-200,-244,-247,-224,-186,-124,-16,98,167,202,230,281,334,365,376,371,362,348,306,244,200,180,164,156,151,114,13,-139,-284,-352,-338,-295,-246,-206,-167,-132,-125,-130,-110,-68,-10,42,44,-4,-46,-22,57,125,151,147,131,106,68,28,5,5,17,10,-27,-87,-131,-128,-69,-9,9,-15,-73,-112,-115,-89,-61,-49,-39,-10,23,45,42,15,-9,-23,-31,-41,-63,-72,-56,-39,-41,-53,-60,-55,-59,-59,-27,37,98,130,132,128,121,103,78,61,55,67,86,81,50,12,-7,-14,-28,-48,-75,-121,-167,-195,-189,-167,-139,-125,-126,-104,-65,-46,-60,-72,-51,12,67,94,103,110,111,88,45,-9,-57,-83,-82,-67,-56,-52,-71,-102,-92,-44,-3,-8,-48,-70,-34,28,64,71,71,85,97,82,56,34,32,42,69,98,107,87,44,4,-19,-12,23,39,17,-19,-63,-97,-112,-109,-118,-124,-96,-40,-2,4};

    complex<float> input [N];
    for (int i = 0; i < 16; i++)
        input [i] = { signal [i], 0 };
    
    complex<float> output [N];

    fft (output, input);

    #define distinctFftCoeficients  ((N + 1) / 2 + 1) // works for both, even and odd numbers

    cout << "distinctFftCoeficients = " << distinctFftCoeficients << endl;

    for (int i = 0; i < distinctFftCoeficients; i++)
        cout << "magnitude [" << i << "] = " << abs (output [i]) / N << endl;
}

void loop () {

}
