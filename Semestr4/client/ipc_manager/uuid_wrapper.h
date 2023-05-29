#ifndef UUID_WRAPPER_H_
#define UUID_WRAPPER_H_

#include <Windows.h>

namespace npipeline_tools {

    class uuid_wrapper {

    private:

        char * _uuid_ptr;

    public:

        uuid_wrapper() {
            UUID uuid;
            UuidCreate(&uuid);
            UuidToString(&uuid, reinterpret_cast<RPC_CSTR *>(&_uuid_ptr));
        }

        char * get() {
            return _uuid_ptr;
        }

        ~uuid_wrapper() {
            RpcStringFree(reinterpret_cast<RPC_CSTR *>(&_uuid_ptr));
        }

    };

}

#endif // UUID_WRAPPER_H_