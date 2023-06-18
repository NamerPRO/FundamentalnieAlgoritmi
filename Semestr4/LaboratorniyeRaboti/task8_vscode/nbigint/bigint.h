#ifndef BIGINT
#define BIGINT

#include <string>

namespace narithmetic {

        template<typename T>
        class bigint {

        public:

                // Comparison implementation BEGIN

                virtual bool equals(
                        T const & other
                        ) const = 0;

                virtual bool operator==(
                        T const & other
                        ) const = 0;

                virtual bool not_equals(
                        T const & other
                        ) const = 0;

                virtual bool operator!=(
                        T const & other
                        ) const = 0;

                virtual bool lower_than(
                        T const & other
                        ) const = 0;

                virtual bool operator<(
                        T const & other
                        ) const = 0;

                virtual bool greater_than(
                        T const & other
                        ) const = 0;

                virtual bool operator>(
                        T const & other
                        ) const = 0;

                virtual bool lower_than_or_equal_to(
                        T const & other
                        ) const = 0;

                virtual bool operator<=(
                        T const & other
                        ) const = 0;

                virtual bool greater_than_or_equal_to(
                        T const & other
                        ) const = 0;

                virtual bool operator>=(
                        T const & other
                        ) const = 0;

                // END comparable implementation

                // Unary minus implementation BEGIN

                virtual T unary_minus() const = 0;

                virtual T operator-() const = 0;

                // END unary minus implementation

                // Addition implementation BEGIN

                virtual T & add(
                        T const & summand
                        ) = 0;

                virtual T & operator+=(
                        T const & summand
                        ) = 0;

                virtual T sum(
                        T const & summand
                        ) = 0;

                virtual T operator+(
                        T const & summand
                        ) = 0;

                // END addition implementation

                // Subtraction implementation BEGIN

                virtual T & subtract(
                        T const & subtrahend
                        ) = 0;

                virtual T & operator-=(
                        T const & subtrahend
                        ) = 0;

                virtual T subtraction(
                        T const & subtrahend
                        ) const = 0;

                virtual T operator-(
                        T const & subtrahend
                        ) const = 0;

                // END subtraction implementation

                // To string implementation BEGIN

                virtual std::string to_string() const = 0;

                // END to string implementation

                // To standard_bigint implementation BEGIN

                virtual T to_bigint(
                        int number
                        ) = 0;

                virtual T to_bigint(
                        long long number
                        ) = 0;

                virtual T to_bigint(
                        std::string const & number
                        ) = 0;

                // END to standard_bigint implementation

                virtual ~bigint() = default;

        };

}

#endif /* BIGINT */
