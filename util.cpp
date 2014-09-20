
#include "util.h"

namespace Util {

    bool Debug(std::string in, std::string message) {
        if (debug) {
            std::cout << "DEBUG: in " << in << ": " << message << std::endl;
        }
    }

    bool debug = false;
}
