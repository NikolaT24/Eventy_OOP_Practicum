#ifndef TICKETED_EVENT_H
#define TICKETED_EVENT_H

#include "Event.h"
#include "SeatingPlan.h"

class TicketedEvent : public Event {
private:
    double price;
    SeatingPlan seating;

public:
    TicketedEvent();
    TicketedEvent(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, double price, const SeatingPlan& seating);
    TicketedEvent(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, EventStatus status, double price, const SeatingPlan& seating);

    EventType getType() const override;
    double getPrice() const;
    const SeatingPlan& getSeating() const;
    SeatingPlan& getSeating();

    bool canBuyGeneral(int count) const;
    bool canBuySeats(const std::vector<Seat>& seats) const;
    bool buyGeneral(int count);
    bool buySeats(const std::vector<Seat>& seats);

    void printInfo() const override;
    std::vector<std::string> toRecord() const override;
};

#endif
