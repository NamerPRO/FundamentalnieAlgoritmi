#ifndef KARACUBA
#define KARACUBA

#include <cmath>
#include <cstring>
#include <limits>
#include <vector>

#include "standard_bigint.h"
#include "bigint_multiplication.h"

namespace narithmetic {

    class karacuba final : public bigint_multiplication<standard_bigint> {

    private:

        long long get_nearest_bigger_or_equal_power_of_two(
            long long number
        );

        long long get_minimal_power_of_two(
            long long number
        );

        void prepare_numbers(
            standard_bigint & number_one,
            standard_bigint & number_two
        );

        long long * to_long_long_array(
            unsigned int * array,
            unsigned int size
        );

    public:

        // standard_bigint operator*(
        //     standard_bigint & right_multiplier
        // ) override;

        standard_bigint multiplication(
            standard_bigint & left_multiplier,
            standard_bigint & right_multiplier
        ) override;

    private:

        standard_bigint & multiply(
            standard_bigint & left_multiplier,
            standard_bigint & right_multiplier
        );

        // void karacuba_multiply(
        //     // standard_bigint & left_multiplier,
        //     // standard_bigint & right_multiplier,
        //     std::vector<long long> & left_multiplier,
        //     std::vector<long long> & right_multiplier,
        //     std::vector<long long> & result,
        //     unsigned int start_index,
        //     unsigned int end_index,
        //     unsigned int actual_size
        // );

        void karacuba_multiply(
            long long * first_factor,
            long long * second_factor,
            long long * intermediate_result,
            unsigned int actual_size
        );

    public:

        // standard_bigint & operator*=(
        //     standard_bigint & right_multiplier
        // );

    };

}

#endif /* KARACUBA */
