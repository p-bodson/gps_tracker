#include "timed_command.h"


char TimedCommand::test() {
    auto func = [] () {return 'a';}; 
    return func();
}
