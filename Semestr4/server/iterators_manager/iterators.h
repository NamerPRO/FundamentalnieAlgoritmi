#ifndef ITERATORS
#define ITERATORS

#include <any>
#include <tuple>

namespace niterators {

    template<typename iterator_type, typename tkey, typename tvalue>
    class iterator {

    public:

        virtual iterator_type & operator++() = 0;

        virtual iterator_type operator++(int) = 0;

        virtual bool equals(
            iterator_type const & other
        ) const = 0;

        bool operator==(
            iterator_type const & other
        ) const {
            return equals(other);
        }

        bool operator!=(
            iterator_type const & other
        ) const {
            return !equals(other);
        }

        virtual std::tuple<unsigned int, tkey, tvalue &> operator*() const = 0;

        virtual ~iterator() = default;

    };

}

#endif /* ITERATORS */
