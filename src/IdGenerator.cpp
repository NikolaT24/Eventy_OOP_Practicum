#include "IdGenerator.h"

IdGenerator::IdGenerator() {
    this->nextValue = 1;
}

IdGenerator::IdGenerator(int startValue) {
    this->nextValue = startValue > 0 ? startValue : 1;
}

int IdGenerator::next() {
    int result = this->nextValue;
    this->nextValue++;
    return result;
}

int IdGenerator::peek() const {
    return this->nextValue;
}

void IdGenerator::setNext(int nextValue) {
    if (nextValue > 0) {
        this->nextValue = nextValue;
    }
}
