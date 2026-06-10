#include "AppState.h"
#include "Admin.h"
#include "DateUtils.h"

AppState::AppState() {
    this->ensureAdmin();
}

void AppState::clear() {
    this->clients.clear();
    this->events.clear();
    this->tickets.clear();
    this->requests.clear();
    this->notifications.clear();
    this->setNextIds(1, 1, 1, 1, 1);
}

void AppState::ensureAdmin() {
    if (this->findClientByUsername(Admin::defaultUsername()) != nullptr) {
        return;
    }

    Client admin(this->nextClientId(), Admin::defaultUsername(), Admin::defaultPassword(), UserRole::Admin);
    this->clients.push_back(admin);
}

std::vector<Client>& AppState::getClients() {
    return this->clients;
}

const std::vector<Client>& AppState::getClients() const {
    return this->clients;
}

std::vector<std::unique_ptr<Event>>& AppState::getEvents() {
    return this->events;
}

const std::vector<std::unique_ptr<Event>>& AppState::getEvents() const {
    return this->events;
}

std::vector<Ticket>& AppState::getTickets() {
    return this->tickets;
}

const std::vector<Ticket>& AppState::getTickets() const {
    return this->tickets;
}

std::vector<Request>& AppState::getRequests() {
    return this->requests;
}

const std::vector<Request>& AppState::getRequests() const {
    return this->requests;
}

std::vector<Notification>& AppState::getNotifications() {
    return this->notifications;
}

const std::vector<Notification>& AppState::getNotifications() const {
    return this->notifications;
}

int AppState::nextClientId() {
    return this->clientIds.next();
}

int AppState::nextEventId() {
    return this->eventIds.next();
}

int AppState::nextTicketId() {
    return this->ticketIds.next();
}

int AppState::nextRequestId() {
    return this->requestIds.next();
}

int AppState::nextNotificationId() {
    return this->notificationIds.next();
}

int AppState::peekNextClientId() const {
    return this->clientIds.peek();
}

int AppState::peekNextEventId() const {
    return this->eventIds.peek();
}

int AppState::peekNextTicketId() const {
    return this->ticketIds.peek();
}

int AppState::peekNextRequestId() const {
    return this->requestIds.peek();
}

int AppState::peekNextNotificationId() const {
    return this->notificationIds.peek();
}

void AppState::setNextIds(int clientId, int eventId, int ticketId, int requestId, int notificationId) {
    this->clientIds.setNext(clientId);
    this->eventIds.setNext(eventId);
    this->ticketIds.setNext(ticketId);
    this->requestIds.setNext(requestId);
    this->notificationIds.setNext(notificationId);
}

Client* AppState::findClientById(int id) {
    for (Client& client : this->clients) {
        if (client.getId() == id) {
            return &client;
        }
    }

    return nullptr;
}

const Client* AppState::findClientById(int id) const {
    for (const Client& client : this->clients) {
        if (client.getId() == id) {
            return &client;
        }
    }

    return nullptr;
}

Client* AppState::findClientByUsername(const std::string& username) {
    for (Client& client : this->clients) {
        if (client.getUsername() == username) {
            return &client;
        }
    }

    return nullptr;
}

const Client* AppState::findClientByUsername(const std::string& username) const {
    for (const Client& client : this->clients) {
        if (client.getUsername() == username) {
            return &client;
        }
    }

    return nullptr;
}

Event* AppState::findEventById(int id) {
    for (std::unique_ptr<Event>& event : this->events) {
        if (event->getId() == id) {
            return event.get();
        }
    }

    return nullptr;
}

const Event* AppState::findEventById(int id) const {
    for (const std::unique_ptr<Event>& event : this->events) {
        if (event->getId() == id) {
            return event.get();
        }
    }

    return nullptr;
}

Request* AppState::findRequestById(int id) {
    for (Request& request : this->requests) {
        if (request.getId() == id) {
            return &request;
        }
    }

    return nullptr;
}

const Request* AppState::findRequestById(int id) const {
    for (const Request& request : this->requests) {
        if (request.getId() == id) {
            return &request;
        }
    }

    return nullptr;
}

void AppState::addNotification(int receiverId, const std::string& message) {
    Notification notification(this->nextNotificationId(), receiverId, message, DateUtils::nowText());
    this->notifications.push_back(notification);
}
