#ifndef INTERPRITATOR
#define INTERPRITATOR

namespace ninterpritator {

    class interpritator {

    public:

        class command {

        public:

            virtual void execute() = 0;

            virtual ~command() = default;

        };

        virtual void interpritate() = 0;

        virtual ~interpritator() = default;

    };

}

#endif /* INTERPRITATOR */
