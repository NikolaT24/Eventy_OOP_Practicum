#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Enums.h"
#include "Notification.h"

class User {
private:
    int id;
    std::string username;
    std::string password;
    std::vector<Notification> notifications;

protected:
    User(int id, std::string username, std::string password,
         std::vector<Notification> notifications = {});

public:
    virtual ~User() = default;

    User(const User&) = default;
    User& operator=(const User&) = default;
    User(User&&) noexcept = default;
    User& operator=(User&&) noexcept = default;

    int getId() const;
    const std::string& getUsername() const;
    const std::string& getPassword() const;
    bool checkPassword(const std::string& candidate) const;

    const std::vector<Notification>& getNotifications() const;
    std::vector<Notification>& getNotifications();
    void addNotification(Notification notification);

    virtual UserRole getRole() const = 0;
    virtual std::unique_ptr<User> clone() const = 0;
};
