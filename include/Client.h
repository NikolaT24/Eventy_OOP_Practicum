#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "Enums.h"

class Client {
private:
    int id;
    std::string username;
    std::string password;
    double balance;
    UserRole role;

public:
    Client();
    Client(
        int id,
        const std::string& username,
        const std::string& password,
        UserRole role = UserRole::Client
    );

    int getId() const;
    std::string getUsername() const;
    bool checkPassword(const std::string& password) const;

    double getBalance() const;
    void addBalance(double amount);
    bool withdraw(double amount);

    UserRole getRole() const;
    bool isAdmin() const;
};

#endif
