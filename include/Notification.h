#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
#include <vector>

class Notification {
private:
    int id;
    int receiverId;
    std::string message;
    std::string createdAt;
    bool read;

public:
    Notification();
    Notification(int id, int receiverId, const std::string& message, const std::string& createdAt, bool read = false);

    int getId() const;
    int getReceiverId() const;
    const std::string& getMessage() const;
    const std::string& getCreatedAt() const;
    bool isRead() const;

    void markAsRead();
    void print() const;
    std::vector<std::string> toRecord() const;
};

#endif
