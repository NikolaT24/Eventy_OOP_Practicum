#include "Repositories.h"
#include "Utils.h"

User* UserRepository::findById(int id) {
    for (auto& user : values) {
        if (user->getId() == id) 
          return user.get();
    }

    return nullptr;
}

const User* UserRepository::findById(int id) const {
    for (const auto& user : values) {
        if (user->getId() == id) 
          return user.get();
    }

    return nullptr;
}

User* UserRepository::findByUsername(const std::string& username) {
    for (auto& user : values) {
        if (user->getUsername() == username) 
          return user.get();
    }

    return nullptr;
}

const User* UserRepository::findByUsername(const std::string& username) const {
    for (const auto& user : values) {
        if (user->getUsername() == username) 
          return user.get();
    }

    return nullptr;
}

Event* EventRepository::findById(int id) {
    for (auto& event : values) {
        if (event->getId() == id) 
          return event.get();
    }

    return nullptr;
}

const Event* EventRepository::findById(int id) const {
    for (const auto& event : values) {
        if (event->getId() == id) 
          return event.get();
    }

    return nullptr;
}

std::vector<Event*> EventRepository::publishedUpcoming() {
    std::vector<Event*> result;

    for (auto& event : values) {
        if (event->isPublished() && utils::isUpcomingDate(event->getDate()))
            result.push_back(event.get());
    }

    return result;
}

std::vector<const Event*> EventRepository::publishedUpcoming() const {
    std::vector<const Event*> result;

    for (const auto& event : values) {
        if (event->isPublished() && utils::isUpcomingDate(event->getDate()))
            result.push_back(event.get());
    }

    return result;
}

std::vector<Event*> EventRepository::ownedBy(int creatorId) {
    std::vector<Event*> result;

    for (auto& event : values) {
        if (event->getCreatorId() == creatorId) 
          result.push_back(event.get());
    }

    return result;
}

std::vector<const Event*> EventRepository::ownedBy(int creatorId) const {
    std::vector<const Event*> result;

    for (const auto& event : values) {
        if (event->getCreatorId() == creatorId) 
          result.push_back(event.get());
    }

    return result;
}

Request* RequestRepository::findById(int id) {
    for (auto& request : values) {
        if (request->getId() == id) 
          return request.get();
    }

    return nullptr;
}

const Request* RequestRepository::findById(int id) const {
    for (const auto& request : values) {
        if (request->getId() == id) 
          return request.get();
    }

    return nullptr;
}

std::vector<Request*> RequestRepository::pendingPublishRequests() {
    std::vector<Request*> result;

    for (auto& request : values) {
        if (request->isPending() && request->isPublishRequest()) 
          result.push_back(request.get());
    }

    return result;
}

std::vector<const Request*> RequestRepository::pendingPublishRequests() const {
    std::vector<const Request*> result;

    for (const auto& request : values) {
        if (request->isPending() && request->isPublishRequest()) 
          result.push_back(request.get());
    }

    return result;
}

std::vector<Request*> RequestRepository::pendingVolunteerApplications(int eventId) {
    std::vector<Request*> result;

    for (auto& request : values) {
        if (request->isPending() && request->isVolunteerApplication() && request->getEventId() == eventId)
            result.push_back(request.get());
    }

    return result;
}

std::vector<const Request*> RequestRepository::pendingVolunteerApplications(int eventId) const {
    std::vector<const Request*> result;

    for (const auto& request : values) {
        if (request->isPending() && request->isVolunteerApplication() && request->getEventId() == eventId)
            result.push_back(request.get());
    }

    return result;
}

bool RequestRepository::hasPendingVolunteerApplication(int requesterId, int eventId) const {
    for (const auto& request : values) {
        if (request->isPending() && request->isVolunteerApplication() &&
            request->getRequesterId() == requesterId && request->getEventId() == eventId)
            return true;
    }

    return false;
}

std::vector<Ticket*> TicketRepository::byOwner(int ownerId) {
    std::vector<Ticket*> result;

    for (Ticket& ticket : values) {
        if (ticket.getOwnerId() == ownerId) 
          result.push_back(&ticket);
    }

    return result;
}

std::vector<const Ticket*> TicketRepository::byOwner(int ownerId) const {
    std::vector<const Ticket*> result;

    for (const Ticket& ticket : values) {
        if (ticket.getOwnerId() == ownerId) 
          result.push_back(&ticket);
    }

    return result;
}

std::vector<Ticket*> TicketRepository::byEvent(int eventId) {
    std::vector<Ticket*> result;

    for (Ticket& ticket : values) {
        if (ticket.getEventId() == eventId) 
          result.push_back(&ticket);
    }

    return result;
}

std::vector<const Ticket*> TicketRepository::byEvent(int eventId) const {
    std::vector<const Ticket*> result;

    for (const Ticket& ticket : values) {
        if (ticket.getEventId() == eventId) 
          result.push_back(&ticket);
    }

    return result;
}

std::vector<Ticket> TicketRepository::takeByEvent(int eventId) {
    std::vector<Ticket> removed;

    for (const Ticket& ticket : values) {
        if (ticket.getEventId() == eventId) 
          removed.push_back(ticket);
    }

    removeIf([&](const Ticket& ticket) {
        return ticket.getEventId() == eventId;
    });

    return removed;
}
