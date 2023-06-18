#ifndef STANDARD_BIGINT
#define STANDARD_BIGINT

#include <iostream>
#include <climits>
#include <stdexcept>
#include <cmath>
#include <vector>

#include "bigint.h"
#include "bigint_multiplication.h"

namespace narithmetic {

        class standard_bigint final : public bigint<standard_bigint>, public bigint_multiplication<standard_bigint> {

        friend class karacuba;
        friend class fmult;

        private:

                static long long const base = 1e8;
                static int const count_of_numbers = 8;

                int _sign;
                std::vector<unsigned int> _parts;

        private:

                explicit standard_bigint(
                        int sign,
                        std::vector<unsigned int> & parts
                ) noexcept;

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

                static bool fits_in_integer(
                        long long amount
                );

                static bool attempt_simple_calculation(
                        standard_bigint & lhs,
                        standard_bigint const & rhs,
                        bool sum = true
                );

                static bool attempt_simple_multiplication(
                        standard_bigint & lhs,
                        standard_bigint const & rhs
                );

        public:

                standard_bigint() noexcept;

                explicit standard_bigint(
                        int number
                ) noexcept;

                explicit standard_bigint(
                        long long number
                ) noexcept;

                explicit standard_bigint(
                        std::string const & number
                );

                standard_bigint(
                        standard_bigint const & number
                );

                standard_bigint & operator=(
                        standard_bigint const & number
                ) noexcept;

                standard_bigint(
                        standard_bigint && number
                ) noexcept;

                standard_bigint & operator=(
                        standard_bigint && number
                ) noexcept;

        public:

                // Comparison implementation BEGIN

                bool equals(
                        standard_bigint const & other
                ) const override;

                bool operator==(
                        standard_bigint const & other
                ) const override;

                bool not_equals(
                        standard_bigint const & other
                ) const override;

                bool operator!=(
                        standard_bigint const & other
                ) const override;

                bool lower_than(
                        standard_bigint const & other
                ) const override;

                bool operator<(
                        standard_bigint const & other
                ) const override;

                bool greater_than(
                        standard_bigint const & other
                ) const override;

                bool operator>(
                        standard_bigint const & other
                ) const override;

                bool lower_than_or_equal_to(
                        standard_bigint const & other
                ) const override;

                bool operator<=(
                        standard_bigint const & other
                ) const override;

                bool greater_than_or_equal_to(
                        standard_bigint const & other
                ) const override;

                bool operator>=(
                        standard_bigint const & other
                ) const override;

                // END comparable implementation

                // Unary minus implementation BEGIN

                standard_bigint unary_minus() const override;

                standard_bigint operator-() const override;
                
                // Multiplication by -1 can alwaysbe done for O(1)
                standard_bigint & multiply_by_minus_one(
                    standard_bigint & number
                );

                // END unary minus implementation

                // Abs implementation begin

                standard_bigint abs(
                        standard_bigint const & number
                );

                // END abs implementation

                // Addition implementation BEGIN

                standard_bigint & add(
                        standard_bigint const & summand
                ) override;

                standard_bigint & operator+=(
                        standard_bigint const & summand
                ) override;

                standard_bigint sum(
                        standard_bigint const & summand
                ) override;

                standard_bigint operator+(
                        standard_bigint const & summand
                ) override;

                // END addition implementation

                // Subtraction implementation BEGIN

                standard_bigint & subtract(
                        standard_bigint const & subtrahend
                ) override;

                standard_bigint & operator-=(
                        standard_bigint const & subtrahend
                ) override;

                standard_bigint subtraction(
                        standard_bigint const & subtrahend
                ) const override;

                standard_bigint operator-(
                        standard_bigint const & subtrahend
                ) const override;

                // END subtraction implementation

                // Classical multiplication implementation BEGIN

                standard_bigint multiplication(
                        standard_bigint & left_multiplier,
                        standard_bigint & right_multiplier
                ) override;

                standard_bigint operator*(
                        standard_bigint & right_multiplier
                );

        private:

                static standard_bigint & multiply(
                        standard_bigint & left_multiplier,
                        standard_bigint & right_multiplier
                );

        public:

                standard_bigint & operator*=(
                        standard_bigint & right_multiplier
                );

                // END classical multiplication implementation

                // To string implementation BEGIN

                std::string to_string() const override;

                // END to string implementation

                // To standard_bigint implementation BEGIN

                standard_bigint to_bigint(
                        int number
                ) override;

                standard_bigint to_bigint(
                        long long number
                ) override;

                standard_bigint to_bigint(
                        std::string const & number
                ) override;

                // END to standard_bigint implementation

        };

        std::ostream & operator<<(
            std::ostream & os,
            standard_bigint const & number
        );

        std::istream & operator>>(
            std::istream & in,
            standard_bigint & number
        );

}

#endif /* STANDARD_BIGINT */
