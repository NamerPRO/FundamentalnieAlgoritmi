#ifndef STANDARD_SENDER_H_
#define STANDARD_SENDER_H_

namespace ntransfer {

    template<typename T>
    class standard_sender {

    public:

        virtual T & add(
            std::vector<std::string> const & parsed_cmd
        ) = 0;

        virtual char * send(
            char * uuid_ptr
        ) = 0;        

    };

}

#endif // STANDARD_SENDER_H_