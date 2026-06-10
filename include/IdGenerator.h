#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

class IdGenerator {
private:
    int nextValue;

public:
    IdGenerator();
    explicit IdGenerator(int startValue);

    int next();
    int peek() const;
    void setNext(int nextValue);
};

#endif
