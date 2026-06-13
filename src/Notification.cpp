#include "Notification.h"

Notification::Notification(int id, std::string message, std::string createdAt, bool read)
    : id(id), message(std::move(message)), createdAt(std::move(createdAt)), read(read) {}

int Notification::getId() const {
    return id;
}

const std::string& Notification::getMessage() const {
    return message;
}

const std::string& Notification::getCreatedAt() const {
    return createdAt;
}

bool Notification::isRead() const {
    return read;
}

void Notification::markAsRead() {
    read = true;
}

std::ostream& operator<<(std::ostream& output, const Notification& notification) {
    output << (notification.read ? "[Read] " : "[Unread] ")
           << notification.createdAt << " | " << notification.message;
    return output;
}
