#pragma once

#include <stdexcept>
#include <string>

class EventyException : public std::runtime_error {
public:
    explicit EventyException(const std::string& message);
};

class ValidationException : public EventyException {
public:
    explicit ValidationException(const std::string& message);
};

class AuthenticationException : public EventyException {
public:
    explicit AuthenticationException(const std::string& message);
};

class AuthorizationException : public EventyException {
public:
    explicit AuthorizationException(const std::string& message);
};

class NotFoundException : public EventyException {
public:
    explicit NotFoundException(const std::string& message);
};

class InvalidStateException : public EventyException {
public:
    explicit InvalidStateException(const std::string& message);
};
