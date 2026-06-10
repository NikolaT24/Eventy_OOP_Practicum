#include "Notification.h"
#include <iostream>

Notification::Notification() {
    this->id = 0;
    this->receiverId = 0;
    this->message = "";
    this->createdAt = "";
    this->read = false;
}

Notification::Notification(int id, int receiverId, const std::string& message, const std::string& createdAt, bool read) {
    this->id = id;
    this->receiverId = receiverId;
    this->message = message;
    this->createdAt = createdAt;
    this->read = read;
}

int Notification::getId() const {
    return this->id;
}

int Notification::getReceiverId() const {
    return this->receiverId;
}

const std::string& Notification::getMessage() const {
    return this->message;
}

const std::string& Notification::getCreatedAt() const {
    return this->createdAt;
}

bool Notification::isRead() const {
    return this->read;
}

void Notification::markAsRead() {
    this->read = true;
}

void Notification::print() const {
    std::cout << (this->read ? "[Read] " : "[Unread] ");
    std::cout << this->createdAt << " - " << this->message << std::endl;
}

std::vector<std::string> Notification::toRecord() const {
    return {
        "NOTIFICATION",
        std::to_string(this->id),
        std::to_string(this->receiverId),
        this->message,
        this->createdAt,
        this->read ? "1" : "0"
    };
}
