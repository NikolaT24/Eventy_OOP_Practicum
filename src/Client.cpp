#include "Client.h"

#include <algorithm>

#include "EventyException.h"

Client::Client(int id, std::string username, std::string password,
               double balance,
               std::vector<int> ticketIds,
               std::vector<int> historyEventIds,
               std::vector<int> createdEventIds,
               std::vector<Notification> notifications)
    : User(id, std::move(username), std::move(password), std::move(notifications)),
      balance(balance),
      ticketIds(std::move(ticketIds)),
      historyEventIds(std::move(historyEventIds)),
      createdEventIds(std::move(createdEventIds)) {
    if (balance < 0) {
        throw ValidationException("Client balance cannot be negative.");
    }
}

void Client::addUnique(std::vector<int>& values, int value) {
    if (std::find(values.begin(), values.end(), value) == values.end()) {
        values.push_back(value);
    }
}

UserRole Client::getRole() const {
    return UserRole::Client;
}

std::unique_ptr<User> Client::clone() const {
    return std::make_unique<Client>(*this);
}

double Client::getBalance() const {
    return balance;
}

const std::vector<int>& Client::getTicketIds() const {
    return ticketIds;
}

const std::vector<int>& Client::getHistoryEventIds() const {
    return historyEventIds;
}

const std::vector<int>& Client::getCreatedEventIds() const {
    return createdEventIds;
}

void Client::addBalance(double amount) {
    if (amount <= 0) {
        throw ValidationException("Balance amount must be positive.");
    }

    balance += amount;
}

void Client::charge(double amount) {
    if (amount < 0) {
        throw ValidationException("Charge amount cannot be negative.");
    }

    if (balance < amount) {
        throw InvalidStateException("Insufficient balance.");
    }

    balance -= amount;
}

void Client::refund(double amount) {
    if (amount < 0) {
        throw ValidationException("Refund amount cannot be negative.");
    }

    balance += amount;
}

void Client::addTicket(int ticketId) {
    addUnique(ticketIds, ticketId);
}

void Client::removeTicket(int ticketId) {
    std::erase(ticketIds, ticketId);
}

void Client::addHistoryEvent(int eventId) {
    addUnique(historyEventIds, eventId);
}

void Client::addCreatedEvent(int eventId) {
    addUnique(createdEventIds, eventId);
}
