#include "Enums.h"

std::string toString(UserRole role) {
    switch (role) {
        case UserRole::Client: return "CLIENT";
        case UserRole::Admin: return "ADMIN";
    }

    return "UNKNOWN";
}

std::string toString(EventStatus status) {
    switch (status) {
        case EventStatus::Pending: return "PENDING";
        case EventStatus::Published: return "PUBLISHED";
        case EventStatus::Cancelled: return "CANCELLED";
    }

    return "UNKNOWN";
}

std::string toString(RequestStatus status) {
    switch (status) {
        case RequestStatus::Pending: return "PENDING";
        case RequestStatus::Approved: return "APPROVED";
        case RequestStatus::Rejected: return "REJECTED";
    }

    return "UNKNOWN";
}

std::string toString(SeatingMode mode) {
    switch (mode) {
        case SeatingMode::GeneralAdmission: return "GENERAL";
        case SeatingMode::AssignedSeats: return "ASSIGNED";
    }

    return "UNKNOWN";
}

std::expected<UserRole, std::string> userRoleFromString(const std::string& value) {
    if (value == "CLIENT") return UserRole::Client;
    if (value == "ADMIN") return UserRole::Admin;
    return std::unexpected("Unknown user role: " + value);
}

std::expected<EventStatus, std::string> eventStatusFromString(const std::string& value) {
    if (value == "PENDING") return EventStatus::Pending;
    if (value == "PUBLISHED") return EventStatus::Published;
    if (value == "CANCELLED") return EventStatus::Cancelled;
    return std::unexpected("Unknown event status: " + value);
}

std::expected<RequestStatus, std::string> requestStatusFromString(const std::string& value) {
    if (value == "PENDING") return RequestStatus::Pending;
    if (value == "APPROVED") return RequestStatus::Approved;
    if (value == "REJECTED") return RequestStatus::Rejected;
    return std::unexpected("Unknown request status: " + value);
}

std::expected<SeatingMode, std::string> seatingModeFromString(const std::string& value) {
    if (value == "GENERAL") return SeatingMode::GeneralAdmission;
    if (value == "ASSIGNED") return SeatingMode::AssignedSeats;
    return std::unexpected("Unknown seating mode: " + value);
}
