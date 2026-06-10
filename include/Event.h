#ifndef EVENT_H
#define EVENT_H

#include <string>
#include "Enums.h"

class Event {
private:
    int id;
    std::string title;
    std::string date;
    std::string address;
    int creatorId;

    EventType type;
    EventStatus status;

    double ticketPrice;
    int capacity;
    int soldTickets;

    std::string volunteerDescription;

public:
    Event();

    Event(
        int id,
        const std::string& title,
        const std::string& date,
        const std::string& address,
        int creatorId,
        EventType type
    );

    int getId() const;
    std::string getTitle() const;
    std::string getDate() const;
    std::string getAddress() const;
    int getCreatorId() const;

    EventType getType() const;
    EventStatus getStatus() const;

    std::string getTypeAsText() const;
    std::string getStatusAsText() const;

    void publish();
    void cancel();

    bool isPublished() const;
    bool isTicketed() const;
    bool isVolunteer() const;

    void setTicketInfo(double price, int capacity);
    double getTicketPrice() const;
    int getCapacity() const;
    int getSoldTickets() const;
    int getAvailableTickets() const;

    bool canSellTickets(int count) const;
    bool sellTickets(int count);

    void setVolunteerDescription(const std::string& description);
    std::string getVolunteerDescription() const;
};

#endif
