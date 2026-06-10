#ifndef EVENTY_SYSTEM_H
#define EVENTY_SYSTEM_H

#include <vector>
#include <string>
#include <sstream>

#include "Client.h"
#include "Event.h"
#include "Ticket.h"
#include "Notification.h"
#include "Request.h"

class EventySystem {
private:
    std::vector<Client> clients;
    std::vector<Event> events;
    std::vector<Ticket> tickets;
    std::vector<Notification> notifications;
    std::vector<Request> requests;

    int nextClientId;
    int nextEventId;
    int nextTicketId;
    int nextNotificationId;
    int nextRequestId;

    int currentClientIndex;
    bool running;

    void initializeAdmin();

    int findClientIndexByUsername(const std::string& username) const;
    int findEventIndexById(int eventId) const;
    int findRequestIndexById(int requestId) const;

    Client* getCurrentClient();

    void addNotification(int receiverId, const std::string& message);

    void handleCommand(const std::string& line);
    void printHelp() const;

    void registerClient(std::stringstream& input);
    void login(std::stringstream& input);
    void logout();

    void showWallet() const;
    void addBalance(std::stringstream& input);

    void createTicketedEvent(std::stringstream& input);
    void createVolunteerEvent(std::stringstream& input);

    void listUpcomingEvents() const;
    void listMyEvents() const;
    void showEventInfo(std::stringstream& input) const;

    void buyTicket(std::stringstream& input);
    void listTickets() const;

    void listNotifications();

    void listRequests() const;
    void approveRequest(std::stringstream& input);
    void rejectRequest(std::stringstream& input);

public:
    EventySystem();

    void run();
};

#endif
