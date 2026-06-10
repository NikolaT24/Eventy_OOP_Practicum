#include "Enums.h"

std::string toString(UserRole role) {
    return role == UserRole::Admin ? "Admin" : "Client";
}

std::string toString(EventType type) {
    return type == EventType::Ticketed ? "Ticketed" : "Volunteer";
}

std::string toString(EventStatus status) {
    if (status == EventStatus::Pending) {
        return "Pending";
    }

    if (status == EventStatus::Published) {
        return "Published";
    }

    return "Cancelled";
}

std::string toString(RequestType type) {
    return type == RequestType::PublishEvent ? "PublishEvent" : "VolunteerApplication";
}

std::string toString(RequestStatus status) {
    if (status == RequestStatus::Pending) {
        return "Pending";
    }

    if (status == RequestStatus::Approved) {
        return "Approved";
    }

    return "Rejected";
}

std::string toString(SeatingMode mode) {
    return mode == SeatingMode::AssignedSeats ? "AssignedSeats" : "GeneralAdmission";
}

std::string toString(ParticipationType type) {
    return type == ParticipationType::Volunteer ? "Volunteer" : "Ticket";
}

std::expected<UserRole, std::string> userRoleFromString(const std::string& text) {
    if (text == "Client") {
        return UserRole::Client;
    }

    if (text == "Admin") {
        return UserRole::Admin;
    }

    return std::unexpected("Invalid user role: " + text);
}

std::expected<EventType, std::string> eventTypeFromString(const std::string& text) {
    if (text == "Ticketed") {
        return EventType::Ticketed;
    }

    if (text == "Volunteer") {
        return EventType::Volunteer;
    }

    return std::unexpected("Invalid event type: " + text);
}

std::expected<EventStatus, std::string> eventStatusFromString(const std::string& text) {
    if (text == "Pending") {
        return EventStatus::Pending;
    }

    if (text == "Published") {
        return EventStatus::Published;
    }

    if (text == "Cancelled") {
        return EventStatus::Cancelled;
    }

    return std::unexpected("Invalid event status: " + text);
}

std::expected<RequestType, std::string> requestTypeFromString(const std::string& text) {
    if (text == "PublishEvent") {
        return RequestType::PublishEvent;
    }

    if (text == "VolunteerApplication") {
        return RequestType::VolunteerApplication;
    }

    return std::unexpected("Invalid request type: " + text);
}

std::expected<RequestStatus, std::string> requestStatusFromString(const std::string& text) {
    if (text == "Pending") {
        return RequestStatus::Pending;
    }

    if (text == "Approved") {
        return RequestStatus::Approved;
    }

    if (text == "Rejected") {
        return RequestStatus::Rejected;
    }

    return std::unexpected("Invalid request status: " + text);
}

std::expected<SeatingMode, std::string> seatingModeFromString(const std::string& text) {
    if (text == "GeneralAdmission") {
        return SeatingMode::GeneralAdmission;
    }

    if (text == "AssignedSeats") {
        return SeatingMode::AssignedSeats;
    }

    return std::unexpected("Invalid seating mode: " + text);
}

std::expected<ParticipationType, std::string> participationTypeFromString(const std::string& text) {
    if (text == "Ticket") {
        return ParticipationType::Ticket;
    }

    if (text == "Volunteer") {
        return ParticipationType::Volunteer;
    }

    return std::unexpected("Invalid participation type: " + text);
}
