#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

template <typename T>
class ValueRepository {
protected:
    std::vector<T> values;

public:
    ValueRepository() = default;

    const std::vector<T>& all() const {
        return values;
    }

    std::vector<T>& all() {
        return values;
    }

    void add(T value) {
        values.push_back(std::move(value));
    }

    template <typename Predicate>
    void removeIf(Predicate predicate) {
        std::erase_if(values, predicate);
    }

    void clear() {
        values.clear();
    }
};

template <typename T>
class PrototypeRepository {
protected:
    std::vector<std::unique_ptr<T>> values;

    void copyFrom(const PrototypeRepository& other) {
        values.reserve(other.values.size());
        for (const auto& value : other.values)
            values.push_back(value->clone());
    }

public:
    PrototypeRepository() = default;

    PrototypeRepository(const PrototypeRepository& other) {
        copyFrom(other);
    }

    PrototypeRepository& operator=(const PrototypeRepository& other) {
        if (this != &other) {
            values.clear();
            copyFrom(other);
        }
        return *this;
    }

    PrototypeRepository(PrototypeRepository&&) noexcept = default;
    PrototypeRepository& operator=(PrototypeRepository&&) noexcept = default;
    virtual ~PrototypeRepository() = default;

    const std::vector<std::unique_ptr<T>>& all() const {
        return values;
    }

    std::vector<std::unique_ptr<T>>& all() {
        return values;
    }

    void add(std::unique_ptr<T> value) {
        values.push_back(std::move(value));
    }

    template <typename Predicate>
    void removeIf(Predicate predicate) {
        std::erase_if(values, [&](const std::unique_ptr<T>& value) {
            return predicate(*value);
        });
    }

    void clear() {
        values.clear();
    }
};
