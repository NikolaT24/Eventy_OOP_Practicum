#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Client.h"
#include "Event.h"
#include "Ticket.h"
#include "Request.h"
#include "Notification.h"
#include "IdGenerator.h"

class AppState {
private:
    std::vector<Client> clients;
    std::vector<std::unique_ptr<Event>> events;
    std::vector<Ticket> tickets;
    std::vector<Request> requests;
    std::vector<Notification> notifications;

    IdGenerator clientIds;
    IdGenerator eventIds;
    IdGenerator ticketIds;
    IdGenerator requestIds;
    IdGenerator notificationIds;

public:
    AppState();
    AppState(const AppState& other);
    AppState& operator=(const AppState& other);
    AppState(AppState&& other) noexcept = default;
    AppState& operator=(AppState&& other) noexcept = default;
    ~AppState() = default;

    void clear();
    void ensureAdmin();

    std::vector<Client>& getClients();
    const std::vector<Client>& getClients() const;

    std::vector<std::unique_ptr<Event>>& getEvents();
    const std::vector<std::unique_ptr<Event>>& getEvents() const;

    std::vector<Ticket>& getTickets();
    const std::vector<Ticket>& getTickets() const;

    std::vector<Request>& getRequests();
    const std::vector<Request>& getRequests() const;

    std::vector<Notification>& getNotifications();
    const std::vector<Notification>& getNotifications() const;

    int nextClientId();
    int nextEventId();
    int nextTicketId();
    int nextRequestId();
    int nextNotificationId();

    int peekNextClientId() const;
    int peekNextEventId() const;
    int peekNextTicketId() const;
    int peekNextRequestId() const;
    int peekNextNotificationId() const;

    void setNextIds(int clientId, int eventId, int ticketId, int requestId, int notificationId);

    Client* findClientById(int id);
    const Client* findClientById(int id) const;
    Client* findClientByUsername(const std::string& username);
    const Client* findClientByUsername(const std::string& username) const;
    Event* findEventById(int id);
    const Event* findEventById(int id) const;
    Request* findRequestById(int id);
    const Request* findRequestById(int id) const;

    void addNotification(int receiverId, const std::string& message);
};