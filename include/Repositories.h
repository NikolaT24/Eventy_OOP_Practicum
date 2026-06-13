#pragma once

#include <string>
#include <vector>
#include "Event.h"
#include "Repository.h"
#include "Request.h"
#include "Ticket.h"
#include "User.h"

class UserRepository final : public PrototypeRepository<User> {
public:
    User* findById(int id);
    const User* findById(int id) const;
    User* findByUsername(const std::string& username);
    const User* findByUsername(const std::string& username) const;
};

class EventRepository final : public PrototypeRepository<Event> {
public:
    Event* findById(int id);
    const Event* findById(int id) const;
    std::vector<Event*> publishedUpcoming();
    std::vector<const Event*> publishedUpcoming() const;
    std::vector<Event*> ownedBy(int creatorId);
    std::vector<const Event*> ownedBy(int creatorId) const;
};

class RequestRepository final : public PrototypeRepository<Request> {
public:
    Request* findById(int id);
    const Request* findById(int id) const;
    std::vector<Request*> pendingPublishRequests();
    std::vector<const Request*> pendingPublishRequests() const;
    std::vector<Request*> pendingVolunteerApplications(int eventId);
    std::vector<const Request*> pendingVolunteerApplications(int eventId) const;
    bool hasPendingVolunteerApplication(int requesterId, int eventId) const;
};

class TicketRepository final : public ValueRepository<Ticket> {
public:
    std::vector<Ticket*> byOwner(int ownerId);
    std::vector<const Ticket*> byOwner(int ownerId) const;
    std::vector<Ticket*> byEvent(int eventId);
    std::vector<const Ticket*> byEvent(int eventId) const;
    std::vector<Ticket> takeByEvent(int eventId);
};
