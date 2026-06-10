#include "Client.h"

Client::Client() {
    this->id = 0;
    this->username = "";
    this->password = "";
    this->balance = 0;
}

Client::Client(int id, const std::string& username, const std::string& password) {
    this->id = id;
    this->username = username;
    this->password = password;
    this->balance = 0;
}

int Client::getId() const {
    return this->id;
}

std::string Client::getUsername() const {
    return this->username;
}

bool Client::checkPassword(const std::string& password) const {
    return this->password == password;
}

double Client::getBalance() const {
    return this->balance;
}

void Client::addBalance(double amount) {
    if (amount > 0) {
        this->balance += amount;
    }
}

bool Client::withdraw(double amount) {
    if (amount <= 0) {
        return false;
    }
    if (this->balance < amount) {
        return false;
    }
    this->balance -= amount;
    return true;
}
