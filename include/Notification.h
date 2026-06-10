#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

class Notification {
private:
    int id;
    int receiverId;
    std::string message;
    bool read;

public:
    Notification();

    Notification(
        int id,
        int receiverId,
        const std::string& message
    );

    int getId() const;
    int getReceiverId() const;
    std::string getMessage() const;
    bool isRead() const;

    void markAsRead();
    void print() const;
};

#endif
