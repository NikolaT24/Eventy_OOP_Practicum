#include "User.h"

User::User(int id, std::string username, std::string password,
           std::vector<Notification> notifications)
    : id(id),
      username(std::move(username)),
      password(std::move(password)),
      notifications(std::move(notifications)) {}

int User::getId() const {
    return id;
}

const std::string& User::getUsername() const {
    return username;
}

const std::string& User::getPassword() const {
    return password;
}

bool User::checkPassword(const std::string& candidate) const {
    return password == candidate;
}

const std::vector<Notification>& User::getNotifications() const {
    return notifications;
}

std::vector<Notification>& User::getNotifications() {
    return notifications;
}

void User::addNotification(Notification notification) {
    notifications.push_back(std::move(notification));
}
