#include "Notification.h"
#include <iostream>

Notification::Notification() {
    this->id = 0;
    this->receiverId = 0;
    this->message = "";
    this->read = false;
}

Notification::Notification(
    int id,
    int receiverId,
    const std::string& message
) {
    this->id = id;
    this->receiverId = receiverId;
    this->message = message;
    this->read = false;
}

int Notification::getId() const {
    return this->id;
}

int Notification::getReceiverId() const {
    return this->receiverId;
}

std::string Notification::getMessage() const {
    return this->message;
}

bool Notification::isRead() const {
    return this->read;
}

void Notification::markAsRead() {
    this->read = true;
}

void Notification::print() const {
    if (this->read) {
        std::cout << "[Read] ";
    }
    else {
        std::cout << "[Unread] ";
    }

    std::cout << this->message << std::endl;
}
