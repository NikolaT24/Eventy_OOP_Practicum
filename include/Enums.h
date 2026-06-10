#ifndef ENUMS_H
#define ENUMS_H

enum class UserRole {
    Guest,
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

enum class RequestStatus {
    Pending,
    Approved,
    Rejected
};

#endif
