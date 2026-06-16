#include <iostream>
#include "EventySystem.h"

int main() {
    try {
        EventySystem::instance().run();
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
