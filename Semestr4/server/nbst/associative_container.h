#ifndef TASK11_ASSOCIATIVE_CONTAINER_H
#define TASK11_ASSOCIATIVE_CONTAINER_H

#include <iostream>

namespace ncontainer {

    template<typename tkey, typename tvalue, bool (*tkey_comporator)(tkey const &, tkey const &)>
    class associative_container {

    public:

        // Standard collection operations implementation

        virtual tvalue & find(
                tkey const & key
        ) = 0;

        virtual void insert(
                tkey const & key,
                tvalue && value
        ) = 0;

        virtual tvalue remove(
                tkey const & key
        ) = 0;

        // ===

        virtual ~associative_container() = default;

        void operator+=(
                std::pair<tkey &&, tvalue &&> node
        ) {
            insert(node.first, std::move(node.second));
        }

        tvalue operator-=(
                tkey const & key
        ) {
            return remove(key);
        }

    };

} // ncontainer

#endif //TASK11_ASSOCIATIVE_CONTAINER_H
