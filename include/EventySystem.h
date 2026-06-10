#ifndef EVENTY_SYSTEM_H
#define EVENTY_SYSTEM_H

#include <vector>
#include <string>
#include "Client.h"
#include "Event.h"

class EventySystem {
private:
    std::vector<Client> clients;
    std::vector<Event> events;

    int nextClientId;
    int nextEventId;
    int currentClientIndex;
    bool running;

    void initializeAdmin();

    int findClientIndexByUsername(const std::string& username) const;
    int findEventIndexById(int eventId) const;

    Client* getCurrentClient();

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

    void listRequests() const;
    void approveRequest(std::stringstream& input);
    void rejectRequest(std::stringstream& input);

public:
    EventySystem();
    void run();
};

#endif
