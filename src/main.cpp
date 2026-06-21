/**
*
* Solution to course project # 8
* Object-Oriented Programming Practicum course
* Faculty of Mathematics and Informatics of Sofia University
* Summer semester 2025/2026
*
* @author Nikola Topalov
* @idnumber 1MI0600248
* @compiler VC
*
* <Eventy>
*
*/

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
