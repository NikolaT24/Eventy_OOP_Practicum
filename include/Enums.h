#ifndef ENUMS_H
#define ENUMS_H

#include <string>
#include <expected>

enum class UserRole {
    Client,
    Admin
};

enum class EventType {
    Ticketed,
    Volunteer
};

enum class EventStatus {
    Pending,
    Published,
    Cancelled
};

enum class RequestType {
    PublishEvent,
    VolunteerApplication
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

enum class ParticipationType {
    Ticket,
    Volunteer
};

std::string toString(UserRole role);
std::string toString(EventType type);
std::string toString(EventStatus status);
std::string toString(RequestType type);
std::string toString(RequestStatus status);
std::string toString(SeatingMode mode);
std::string toString(ParticipationType type);

std::expected<UserRole, std::string> userRoleFromString(const std::string& text);
std::expected<EventType, std::string> eventTypeFromString(const std::string& text);
std::expected<EventStatus, std::string> eventStatusFromString(const std::string& text);
std::expected<RequestType, std::string> requestTypeFromString(const std::string& text);
std::expected<RequestStatus, std::string> requestStatusFromString(const std::string& text);
std::expected<SeatingMode, std::string> seatingModeFromString(const std::string& text);
std::expected<ParticipationType, std::string> participationTypeFromString(const std::string& text);

#endif
