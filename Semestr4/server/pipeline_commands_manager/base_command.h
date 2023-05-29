#ifndef BASE_COMMAND_H_
#define BASE_COMMAND_H_

namespace npipeline_tools {

    class command {

    public:

        virtual void execute() = 0;

        virtual ~command() = default;

    };

}

#endif // BASE_COMMAND_H_