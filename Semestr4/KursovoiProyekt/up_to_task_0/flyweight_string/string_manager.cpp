#include "./string_manager.h"
#include <cstddef>

namespace nstring {

    std::string const & string_factory::get_string(
        std::string const & str
    ) {
        if (_string_pool.contains(str)) {
            return _string_pool[str];
        }
        return _string_pool[str] = str;
    }

    std::string const & string_factory::get_string(
        std::string && str
    ) {
        if (_string_pool.contains(str)) {
            return _string_pool[str];
        }
        return _string_pool[str] = std::move(str);
    }

    string_factory & string_factory::get_instance() {
        static string_factory singelton_object;
        return singelton_object;
    }

}