#ifndef EVENTY_SYSTEM_H
#define EVENTY_SYSTEM_H

#include "AppState.h"
#include "StorageService.h"
#include "CommandParser.h"

class EventySystem {
private:
    AppState state;
    StorageService storage;
    int currentUserId;
    int managedEventId;
    bool running;

    Client* currentUser();
    const Client* currentUser() const;
    Event* managedEvent();
    const Event* managedEvent() const;

    bool isLoggedIn() const;
    bool isAdmin() const;
    bool isClient() const;

    void handleCommand(const ParsedCommand& command);
    void handleGuestCommand(const ParsedCommand& command);
    void handleAdminCommand(const ParsedCommand& command);
    void handleClientCommand(const ParsedCommand& command);
    void handleEventManagementCommand(const ParsedCommand& command);

    void printGuestHelp() const;
    void printClientHelp() const;
    void printAdminHelp() const;
    void printEventManagementHelp() const;

    void registerClient(const std::vector<std::string>& args);
    void login(const std::vector<std::string>& args);
    void logout();
    void exitApplication();

    void showWallet() const;
    void addBalance(const std::vector<std::string>& args);

    void createTicketedEvent(const std::vector<std::string>& args);
    void createSeatedEvent(const std::vector<std::string>& args);
    void createVolunteerEvent(const std::vector<std::string>& args);

    void listUpcomingEvents() const;
    void listMyEvents() const;
    void showEventInfo(const std::vector<std::string>& args) const;
    void showCurrentEventInfo() const;
    void showSeating(const std::vector<std::string>& args) const;

    void buyTicket(const std::vector<std::string>& args);
    void listTickets() const;
    void listHistory() const;

    void submitVolunteerApplication(const std::vector<std::string>& args);
    void listNotifications();

    void enterEvent(const std::vector<std::string>& args);
    void exitEvent();
    void cancelCurrentEvent(const std::vector<std::string>& args);
    void listVolunteerApplications() const;
    void approveVolunteerApplication(const std::vector<std::string>& args);
    void rejectVolunteerApplication(const std::vector<std::string>& args);
    void closeVolunteerApplications();
    void listParticipants() const;

    void listPublishRequests() const;
    void approvePublishRequest(const std::vector<std::string>& args);
    void rejectPublishRequest(const std::vector<std::string>& args);

    bool hasActiveVolunteerApplication(int userId, int eventId) const;
    void refundTicketsForCancelledEvent(int eventId, const std::string& reason);

public:
    EventySystem();
    void run();
};

#endif
