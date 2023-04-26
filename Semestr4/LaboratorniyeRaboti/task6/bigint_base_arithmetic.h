#ifndef TASK6_BIGINT_BASE_ARITHMETIC_H
#define TASK6_BIGINT_BASE_ARITHMETIC_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <climits>

#include "bigint.h"

namespace ninteger {

    class bigint_base_arithmetic final : public bigint<bigint_base_arithmetic> {

    private:

        long long const base = 1e9;

        int _sign;
        std::vector<unsigned int> _parts;

        explicit bigint_base_arithmetic(
                int sign,
                std::vector<unsigned int> & parts
                ) noexcept;

    private:

        static std::string & satiate_with_zeros(
                std::string && number
                );

        static bool parse_long_long(
                long long number,
                std::vector<unsigned int> & parts
                ) noexcept;

        static bool parse_string(
                std::string const & number,
                std::vector<unsigned int> & parts
                );

    public:

        explicit bigint_base_arithmetic() noexcept;

        explicit bigint_base_arithmetic(
                int number
                ) noexcept;

        explicit bigint_base_arithmetic(
                long long number
                ) noexcept;

        explicit bigint_base_arithmetic(
                std::string const & number
                );

    public:

        bigint_base_arithmetic(
                bigint_base_arithmetic const & number
                );

        bigint_base_arithmetic & operator=(
                bigint_base_arithmetic && number
                ) noexcept;

    public:

        bigint_base_arithmetic to_bigint(
                int number
                ) override;

        bigint_base_arithmetic to_bigint(
                long long number
                ) override;

        bigint_base_arithmetic to_bigint(
                std::string const & number
                ) override;

        std::string to_string() const override;

    public:

        bool equals(
                bigint_base_arithmetic const & other
                ) const override;

        bool operator==(
                bigint_base_arithmetic const & other
                ) const override;

        bool not_equals(
                bigint_base_arithmetic const & other
                ) const override;

        bool operator!=(
                bigint_base_arithmetic const & other
                ) const override;

        bool lower_than(
                bigint_base_arithmetic const & other
                ) const override;

        bool operator<(
                bigint_base_arithmetic const & other
                ) const override;

        bool greater_than(
                bigint_base_arithmetic const & other
                ) const override;

        bool operator>(
                bigint_base_arithmetic const & other
                ) const override;

        bool lower_than_or_equal_to(
                bigint_base_arithmetic const & other
                ) const override;

        bool operator<=(
                bigint_base_arithmetic const & other
                ) const override;

        bool greater_than_or_equal_to(
                bigint_base_arithmetic const & other
                ) const override;

        bool operator>=(
                bigint_base_arithmetic const & other
                ) const override;

    private:

        static bool fits_in_integer(
                long long amount
                );

        static bool attempt_simple_calculation(
                bigint_base_arithmetic & lhs,
                bigint_base_arithmetic const & rhs,
                bool sum = true
        );

    public:

        static bigint_base_arithmetic & multiply_by_minus_one(
                bigint_base_arithmetic & number
                );

        bigint_base_arithmetic unary_minus() const override;

        bigint_base_arithmetic operator-() const override;

        static bigint_base_arithmetic abs(
                bigint_base_arithmetic const & number
                );

        bigint_base_arithmetic & add(
                bigint_base_arithmetic const & summand
                ) override;

        bigint_base_arithmetic & operator+=(
                bigint_base_arithmetic const & summand
                ) override;

        bigint_base_arithmetic sum(
                bigint_base_arithmetic const & summand
                ) override;

        bigint_base_arithmetic operator+(
                bigint_base_arithmetic const & summand
                ) override;

        bigint_base_arithmetic & subtract(
                bigint_base_arithmetic const & subtrahend
                ) override;

        bigint_base_arithmetic & operator-=(
                bigint_base_arithmetic const & subtrahend
                ) override;

        bigint_base_arithmetic subtraction(
                bigint_base_arithmetic const & subtrahend
                ) const override;

        bigint_base_arithmetic operator-(
                bigint_base_arithmetic const & subtrahend
                ) const override;

        ~bigint_base_arithmetic() override = default;

    };

    std::ostream & operator<<(
            std::ostream & os,
            ninteger::bigint_base_arithmetic const & number
            );

    std::istream & operator>>(
            std::istream & in,
            ninteger::bigint_base_arithmetic & number
            );

} // ninteger

#endif //TASK6_BIGINT_BASE_ARITHMETIC_H
