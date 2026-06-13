#include "EventyException.h"

EventyException::EventyException(const std::string& message)
    : std::runtime_error(message) {}

ValidationException::ValidationException(const std::string& message)
    : EventyException(message) {}

AuthenticationException::AuthenticationException(const std::string& message)
    : EventyException(message) {}

AuthorizationException::AuthorizationException(const std::string& message)
    : EventyException(message) {}

NotFoundException::NotFoundException(const std::string& message)
    : EventyException(message) {}

InvalidStateException::InvalidStateException(const std::string& message)
    : EventyException(message) {}
