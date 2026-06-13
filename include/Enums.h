#pragma once

#include <expected>
#include <string>

enum class UserRole {
    Client,
    Admin
};

enum class EventStatus {
    Pending,
    Published,
    Cancelled
};

enum class RequestStatus {
    Pending,
    Approved,
    Rejected
};

enum class SeatingMode {
    GeneralAdmission,
    AssignedSeats
};

std::string toString(UserRole role);
std::string toString(EventStatus status);
std::string toString(RequestStatus status);
std::string toString(SeatingMode mode);

std::expected<UserRole, std::string> userRoleFromString(const std::string& value);
std::expected<EventStatus, std::string> eventStatusFromString(const std::string& value);
std::expected<RequestStatus, std::string> requestStatusFromString(const std::string& value);
std::expected<SeatingMode, std::string> seatingModeFromString(const std::string& value);
