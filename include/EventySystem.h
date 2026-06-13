#pragma once

#include <string>
#include <vector>
#include "Command.h"
#include "Repositories.h"
#include "StorageService.h"

class Admin;
class Client;
class User;
class Event;
class VolunteerEvent;

class EventySystem final {
private:
    UserRepository users;
    EventRepository events;
    RequestRepository requests;
    TicketRepository tickets;

    StorageService storage;
    IdState ids;

    CommandRegistry guestCommands;
    CommandRegistry clientCommands;
    CommandRegistry adminCommands;
    CommandRegistry managementCommands;

    int currentUserId = -1;
    int managedEventId = -1;
    bool running = true;

    EventySystem();

    void configureCommands();
    void ensureAdmin();
    void archivePastTickets();

    User* currentUser();
    const User* currentUser() const;
    Client* currentClient();
    const Client* currentClient() const;
    Admin* currentAdmin();
    const Admin* currentAdmin() const;
    Event* managedEvent();
    const Event* managedEvent() const;

    void notify(int userId, const std::string& message);
    void executeCommand(const ParsedCommand& command);

    void printGuestHelp(const std::vector<std::string>& arguments) const;
    void printClientHelp(const std::vector<std::string>& arguments) const;
    void printAdminHelp(const std::vector<std::string>& arguments) const;
    void printManagementHelp(const std::vector<std::string>& arguments) const;

    void registerClient(const std::vector<std::string>& arguments);
    void login(const std::vector<std::string>& arguments);
    void logout(const std::vector<std::string>& arguments);
    void exitApplication(const std::vector<std::string>& arguments);

    void showWallet(const std::vector<std::string>& arguments) const;
    void addBalance(const std::vector<std::string>& arguments);

    void createEvent(const std::vector<std::string>& arguments);
    void createTicketedAlias(const std::vector<std::string>& arguments);
    void createSeatedAlias(const std::vector<std::string>& arguments);
    void createVolunteerAlias(const std::vector<std::string>& arguments);

    void listUpcomingEvents(const std::vector<std::string>& arguments) const;
    void listMyEvents(const std::vector<std::string>& arguments) const;
    void showEventInfo(const std::vector<std::string>& arguments) const;
    void showCurrentEventInfo(const std::vector<std::string>& arguments) const;
    void showSeating(const std::vector<std::string>& arguments) const;

    void buyTicket(const std::vector<std::string>& arguments);
    void listTickets(const std::vector<std::string>& arguments) const;
    void listHistory(const std::vector<std::string>& arguments) const;

    void submitVolunteerApplication(const std::vector<std::string>& arguments);
    void listNotifications(const std::vector<std::string>& arguments);

    void enterEvent(const std::vector<std::string>& arguments);
    void exitEvent(const std::vector<std::string>& arguments);
    void cancelCurrentEvent(const std::vector<std::string>& arguments);
    void listVolunteerApplications(const std::vector<std::string>& arguments) const;
    void approveVolunteerApplication(const std::vector<std::string>& arguments);
    void rejectVolunteerApplication(const std::vector<std::string>& arguments);
    void closeVolunteerApplications(const std::vector<std::string>& arguments);
    void listParticipants(const std::vector<std::string>& arguments) const;

    void listPublishRequests(const std::vector<std::string>& arguments) const;
    void approvePublishRequest(const std::vector<std::string>& arguments);
    void rejectPublishRequest(const std::vector<std::string>& arguments);

    void refundTicketsForCancelledEvent(int eventId, const std::string& reason);

public:
    EventySystem(const EventySystem&) = delete;
    EventySystem& operator=(const EventySystem&) = delete;

    static EventySystem& instance();
    void run();
};
