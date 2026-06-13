#pragma once

#include <ostream>
#include <string>

class Notification {
private:
    int id;
    std::string message;
    std::string createdAt;
    bool read;

public:
    Notification(int id, std::string message, std::string createdAt, bool read = false);

    int getId() const;
    const std::string& getMessage() const;
    const std::string& getCreatedAt() const;
    bool isRead() const;

    void markAsRead();

    friend std::ostream& operator<<(std::ostream& output, const Notification& notification);
};
