#include "fft.h"
#include "standard_bigint.h"
#include <climits>
#include <vector>

namespace narithmetic {

    standard_bigint fmult::multiplication(
        standard_bigint & left_multiplier,
        standard_bigint & right_multiplier
    ) {
        bool is_result_negative = left_multiplier._sign < 0 ^ right_multiplier._sign < 0;

        std::vector<unsigned int> && prepared_left_multiplier{};
        std::vector<unsigned int> && prepared_right_multiplier{};
        if (left_multiplier._parts.empty()) {
            if (left_multiplier._sign == INT_MIN) {
                prepared_left_multiplier.push_back(INT_MAX + 1ul);
            } else {
                prepared_left_multiplier.push_back(std::abs(left_multiplier._sign));
            }
        } else {
            prepared_left_multiplier = std::move(left_multiplier._parts);
        }
        if (right_multiplier._parts.empty()) {
            if (right_multiplier._sign == INT_MIN) {
                prepared_right_multiplier.push_back(INT_MAX + 1ul);
            } else {
                prepared_right_multiplier.push_back(std::abs(right_multiplier._sign));
            }
        } else {
            prepared_right_multiplier = std::move(right_multiplier._parts);
        }
        std::vector<unsigned int> result{};
        multiply(prepared_left_multiplier, prepared_right_multiplier, result);

        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }

        standard_bigint result_as_bigint{};

        if (result.size() == 1) {
            result_as_bigint = standard_bigint(static_cast<long long>(result[0]));
            result_as_bigint._sign *= is_result_negative ? -1 : 1;
        } else {
            result_as_bigint = standard_bigint(is_result_negative ? -1 : 1, result);
        }

        return result_as_bigint;
    }

    void fmult::fft(
        std::vector<base> & fft_storage,
        bool invert
    ) {
        unsigned int n = fft_storage.size();
    
        for (unsigned int i = 1, j = 0; i < n; ++i) {
            int bit = n >> 1;
            for (; j >= bit; bit >>= 1) {
                j -= bit;
            }
            j += bit;
            if (i < j) {
                swap(fft_storage[i], fft_storage[j]);
            }
        }
    
        for (unsigned int len = 2; len <= n; len <<= 1) {
            double angle = 2 * PI / len * (invert ? -1 : 1);
            base wlen(cos(angle), sin(angle));
            for (unsigned int i = 0; i < n; i += len) {
                base w(1);
                for (unsigned int j=0; j < len / 2; ++j) {
                    base u = fft_storage[i + j];
                    base v = fft_storage[i + j + len / 2] * w;

                    fft_storage[i + j] = u + v;
                    fft_storage[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }
        if (invert) {
            for (int i=0; i<n; ++i) {
                fft_storage[i] /= n;
            }
        }
    }

    // void fmult::fft(
    //     std::vector<base> & a,
    //     bool invert
    // ) {
    //     unsigned int n = a.size();
    //     if (n == 1)  return;
    
    //     std::vector<base> a0 (n/2), a1 (n/2);
    //     for (unsigned int i=0, j=0; i<n; i+=2, ++j) {
    //         a0[j] = a[i];
    //         a1[j] = a[i+1];
    //     }
    //     fft (a0, invert);
    //     fft (a1, invert);
    
    //     double ang = 2*PI/n * (invert ? -1 : 1);
    //     base w (1),  wn (cos(ang), sin(ang));
    //     for (unsigned int i=0; i<n/2; ++i) {
    //         a[i] = a0[i] + w * a1[i];
    //         a[i+n/2] = a0[i] - w * a1[i];
    //         if (invert)
    //             a[i] /= 2,  a[i+n/2] /= 2;
    //         w *= wn;
    //     }
    // }

    void fmult::multiply(
        std::vector<unsigned int> const & left_multiplier,
        std::vector<unsigned int> const & right_multiplier,
        std::vector<unsigned int> & res
    ) {
        std::vector<base> fft_left_multiplier(left_multiplier.begin(), left_multiplier.end());
        std::vector<base> fft_right_multiplier(right_multiplier.begin(), right_multiplier.end());
        
        unsigned int required_power_of_two = 1;

        while (required_power_of_two < std::max(fft_left_multiplier.size(), fft_right_multiplier.size())) {
            required_power_of_two <<= 1;
        }

        required_power_of_two <<= 1;

        fft_left_multiplier.resize(required_power_of_two);
        fft_right_multiplier.resize(required_power_of_two);
    
        fft(fft_left_multiplier, false);
        fft(fft_right_multiplier, false);

        for (unsigned int i = 0; i < required_power_of_two; ++i) {
            fft_left_multiplier[i] *= fft_right_multiplier[i];
        }

        fft (fft_left_multiplier, true);
        res.resize(required_power_of_two);

        unsigned long long carry = 0;
        unsigned long long intermediate_result = 0;
        for (unsigned int i=0; i < required_power_of_two; ++i) {
            intermediate_result = static_cast<unsigned long long>(fft_left_multiplier[i].real() + 0.5) + carry;
            carry = intermediate_result / standard_bigint::base;
            res[i] = intermediate_result % standard_bigint::base;
        }
    }

}