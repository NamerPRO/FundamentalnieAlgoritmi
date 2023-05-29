#ifndef STRING_MANAGER
#define STRING_MANAGER

#include <string>
#include <unordered_map>

namespace nstring {

    class string_factory {

    private:

        string_factory() = default;

        std::unordered_map<std::string, std::string> _string_pool;

    public:

        std::string const & get_string(
            std::string const & str
        );

        std::string const & get_string(
            std::string && str
        );

        static string_factory & get_instance();

        string_factory(
            string_factory const & unit
        ) = delete;

        string_factory & operator=(
            string_factory const & unit
        ) = delete;

    };

}

#endif /* STRING_MANAGER */
