//#include "associative_container.h"
//
//template<
//        typename tkey,
//        typename tvalue,
//        typename tkey_comporator
//        >
//void ncontainer::associative_container<tkey, tvalue, tkey_comporator>::operator+=(
//        std::pair<tkey const &, tvalue const &> node
//        ) {
//    insert(node.first, node.second);
//}
//
//template<
//        typename tkey,
//        typename tvalue,
//        typename tkey_comporator
//        >
//tvalue && ncontainer::associative_container<tkey, tvalue, tkey_comporator>::operator-=(
//        tkey const & key
//        ) {
//    return remove(key);
//}