#ifndef BIGINT_MULTIPLICATION
#define BIGINT_MULTIPLICATION

namespace narithmetic {

    template<typename T>
    class bigint_multiplication {

        virtual T multiplication(
            T & left_multiplier,
            T & right_multiplier
        ) = 0;

    };

}

#endif /* BIGINT_MULTIPLICATION */
