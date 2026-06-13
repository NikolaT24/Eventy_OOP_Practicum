#pragma once

#include <memory>
#include <vector>
#include "User.h"

class Client final : public User {
private:
    double balance;
    std::vector<int> ticketIds;
    std::vector<int> historyEventIds;
    std::vector<int> createdEventIds;

    static void addUnique(std::vector<int>& values, int value);

public:
    Client(int id, std::string username, std::string password,
           double balance = 0.0,
           std::vector<int> ticketIds = {},
           std::vector<int> historyEventIds = {},
           std::vector<int> createdEventIds = {},
           std::vector<Notification> notifications = {});

    UserRole getRole() const override;
    std::unique_ptr<User> clone() const override;

    double getBalance() const;
    const std::vector<int>& getTicketIds() const;
    const std::vector<int>& getHistoryEventIds() const;
    const std::vector<int>& getCreatedEventIds() const;

    void addBalance(double amount);
    void charge(double amount);
    void refund(double amount);

    void addTicket(int ticketId);
    void removeTicket(int ticketId);
    void addHistoryEvent(int eventId);
    void addCreatedEvent(int eventId);
};
