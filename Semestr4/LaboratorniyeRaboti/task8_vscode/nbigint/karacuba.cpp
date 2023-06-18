#include "karacuba.h"

namespace narithmetic {

    standard_bigint karacuba::multiplication(
        standard_bigint & left_multiplier,
        standard_bigint & right_multiplier
    ) {
        standard_bigint result(left_multiplier);
        multiply(result, right_multiplier);
        return result;
    }

    standard_bigint & karacuba::multiply(
        standard_bigint & left_multiplier,
        standard_bigint & right_multiplier
    ) {
        if (left_multiplier._parts.size() <= 64 && right_multiplier._parts.size() <= 64) {
            return standard_bigint::multiply(left_multiplier, right_multiplier);
        }
        short result_sign = (left_multiplier._sign < 0 ^ right_multiplier._sign < 0) ? -1 : 1;
        unsigned int previous_right_multiplier_size = right_multiplier._parts.size();
        prepare_numbers(left_multiplier, right_multiplier);
        long long * first_factor = to_long_long_array(&left_multiplier._parts[0], left_multiplier._parts.size());
        long long * second_factor = to_long_long_array(&right_multiplier._parts[0], right_multiplier._parts.size());
        long long maximum_result_size = left_multiplier._parts.size() + right_multiplier._parts.size();
        long long * result = (long long *)malloc(sizeof(long long) * maximum_result_size);
        std::memset(result, 0, sizeof(long long) * maximum_result_size);
        karacuba_multiply(first_factor, second_factor, result, left_multiplier._parts.size());

        long long carry = 0;
        for (unsigned int i = 0; i < maximum_result_size || carry; ++i) {
            result[i] += carry;
            carry = result[i] / standard_bigint::base;
            result[i] %= standard_bigint::base;
        }

        std::vector<unsigned int> result_vector(result, result + maximum_result_size);
        left_multiplier._parts = std::move(result_vector);
        left_multiplier._sign = result_sign;

        while (left_multiplier._parts.size() > 1 && left_multiplier._parts.back() == 0) {
            left_multiplier._parts.pop_back();
        }

        right_multiplier._parts.resize(previous_right_multiplier_size);

        return left_multiplier;
    }

    long long * karacuba::to_long_long_array(
        unsigned int * array,
        unsigned int size
    ) {
        long long * new_array = (long long *)malloc(sizeof(long long) * size);
        for (unsigned int i = 0; i < size; ++i) {
            new_array[i] = array[i];
        }
        return new_array;
    }

    void karacuba::karacuba_multiply(
        long long * first_factor,
        long long * second_factor,
        long long * intermediate_result,
        unsigned int actual_size
    ) {
        if (actual_size <= 64) {
            for (unsigned int i = 0; i < actual_size; ++i) {
                for (unsigned int j = 0; j < actual_size; ++j) {                    
                    intermediate_result[i + j] += first_factor[i] * second_factor[j];
                }
            }
        } else {
            unsigned int new_size = actual_size / 2;
            long long a1_plus_b1[new_size], a2_plus_b2[new_size], a1_plus_b1_mult_a2_plus_b2[actual_size];
            std::memset(a1_plus_b1, 0, sizeof(long long) * new_size);
            std::memset(a2_plus_b2, 0, sizeof(long long) * new_size);
            std::memset(a1_plus_b1_mult_a2_plus_b2, 0, sizeof(long long) * actual_size);
            for (unsigned int i = 0; i < new_size; ++i) {
                a1_plus_b1[i] = first_factor[i] + first_factor[new_size + i];
                a2_plus_b2[i] = second_factor[i] + second_factor[new_size + i];
            }
            karacuba_multiply(a1_plus_b1, a2_plus_b2, a1_plus_b1_mult_a2_plus_b2, new_size); // считает t
            karacuba_multiply(first_factor, second_factor, intermediate_result, new_size); // считает p1
            karacuba_multiply(first_factor + new_size, second_factor + new_size, intermediate_result + actual_size, new_size); // считает p2
            long long * t1 = a1_plus_b1_mult_a2_plus_b2;
            long long * t2 = a1_plus_b1_mult_a2_plus_b2 + new_size;
            long long * s1 = intermediate_result;
            long long * s2 = intermediate_result + new_size;
            long long * s3 = intermediate_result + 2 * new_size;
            long long * s4 = intermediate_result + 3 * new_size;
            for (unsigned int i = 0; i < new_size; ++i) {
                long long c1 = s2[i] + t1[i] - s1[i] - s3[i];
                long long c2 = s3[i] + t2[i] - s2[i] - s4[i];
                intermediate_result[new_size + i] = c1;
                intermediate_result[actual_size + i] = c2;
            }
        }
    }

    // void karacuba::karacuba_multiply(
    //     // standard_bigint & left_multiplier,
    //     // standard_bigint & right_multiplier,
    //     std::vector<long long> & left_multiplier,
    //     std::vector<long long> & right_multiplier,
    //     std::vector<long long> & result,
    //     unsigned int start_index,
    //     unsigned int end_index,
    //     unsigned int actual_size
    // ) {
    //     // if (left_multiplier._parts.size() <= 64 && right_multiplier._parts.size() <= 64) {
    //         // return standard_bigint::multiply(left_multiplier, right_multiplier);
    //     // }
    //     if (actual_size <= 64) {
    //         for (unsigned int i = 0; i < actual_size; ++i) {
    //             for (unsigned int j = 0; j < actual_size; ++j) {
    //                 result[i + j] += left_multiplier[start_index + i] * right_multiplier[start_index + j];
    //             }
    //         }
    //         return;
    //     }
    //     unsigned int new_size = actual_size / 2;

    //     std::vector<long long> p1(new_size), p2(new_size);
    //     for (unsigned int i = 0; i < new_size; ++i) {
    //         p1[i] = left_multiplier[i] + left_multiplier[i + new_size];
    //         p2[i] = right_multiplier[i] + right_multiplier[i + new_size];
    //     }

    //     karacuba_multiply(p1, p2, w3, 0, new_size - 1, new_size); // calculate w3
    //     karacuba_multiply(left_multiplier, right_multiplier, result, start_index, start_index + new_size - 1, new_size); // calculate w1
    //     karacuba_multiply(left_multiplier, right_multiplier, result, start_index + new_size, end_index, new_size); // calcuate w2



    // }

    // standard_bigint & karacuba::operator*=(
    //     standard_bigint & right_multiplier
    // ) {
    //     multiply(*this, right_multiplier);
    // }

    void karacuba::prepare_numbers(
        standard_bigint & number_one,
        standard_bigint & number_two
    ) {
        if (number_two._parts.empty()) {
            std::swap(number_one, number_two);
        }
        if (number_one._parts.empty()) {
            number_one._parts.push_back(std::abs(number_one._sign));
        }
        long long power_one = get_nearest_bigger_or_equal_power_of_two(number_one._parts.size());
        long long power_two = get_nearest_bigger_or_equal_power_of_two(number_two._parts.size());
        long long max_power = std::max(power_one, power_two);
        
        number_one._parts.resize(1ll << max_power, 0);
        number_two._parts.resize(1ll << max_power, 0);

        // while (get_minimal_power_of_two(number_one._parts.size()) != max_power || get_minimal_power_of_two(number_two._parts.size()) != max_power) {
        //     if (get_minimal_power_of_two(number_one._parts.size()) != max_power) {
        //         number_one._parts.push_back(0);
        //     }
        //     if (get_minimal_power_of_two(number_two._parts.size()) != max_power) {
        //         number_two._parts.push_back(0);
        //     }
        // }
    }

    long long karacuba::get_minimal_power_of_two(
        long long number
    ) {
        double power_of_two = std::log2(number) + std::numeric_limits<double>::epsilon();
        return static_cast<long long>(power_of_two);
    }

    long long karacuba::get_nearest_bigger_or_equal_power_of_two(
        long long number
    ) {
        double power_of_two = std::log2(number) + std::numeric_limits<double>::epsilon();
        long long minimal_power_of_two = static_cast<long long>(power_of_two);
        if (std::abs(power_of_two - minimal_power_of_two) < std::numeric_limits<double>::epsilon()) {
            return minimal_power_of_two;
        }
        return minimal_power_of_two + 1;
    }

}