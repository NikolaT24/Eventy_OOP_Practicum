#ifndef TICKET_H
#define TICKET_H

#include <string>

class Ticket {
private:
    int id;
    int ownerId;
    int eventId;
    std::string eventTitle;
    int count;
    double totalPrice;

public:
    Ticket();

    Ticket(
        int id,
        int ownerId,
        int eventId,
        const std::string& eventTitle,
        int count,
        double totalPrice
    );

    int getId() const;
    int getOwnerId() const;
    int getEventId() const;
    std::string getEventTitle() const;
    int getCount() const;
    double getTotalPrice() const;

    void print() const;
};

#endif
