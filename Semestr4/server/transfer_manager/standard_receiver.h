#ifndef STANDARD_RECEIVER_H_
#define STANDARD_RECEIVER_H_

#include <string>

namespace ntransfer {

    class standard_receiver {

    public:

        virtual std::string receive(
            char * uuid_ptr
        ) = 0;

    };

}

#endif // STANDARD_RECEIVER_H_