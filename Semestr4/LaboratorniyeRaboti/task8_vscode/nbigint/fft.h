#ifndef FFT
#define FFT

#include <complex>
#include <vector>
#include <cmath>

#include "standard_bigint.h"

namespace narithmetic {

    class fmult final : public bigint_multiplication<standard_bigint> {

    private:

        typedef std::complex<long double> base;
        double const PI = std::acos(-1);

    public:

        standard_bigint multiplication(
            standard_bigint & left_multiplier,
            standard_bigint & right_multiplier
        ) override;

    private:

        void fft(
            std::vector<base> & a,
            bool invert
        );
        
        void multiply (
            std::vector<unsigned int> const & a,
            std::vector<unsigned int> const & b,
            std::vector<unsigned int> & res
        );

    };
}

#endif /* FFT */
