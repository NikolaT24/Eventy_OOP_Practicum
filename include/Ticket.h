#ifndef TICKET_H
#define TICKET_H

#include <string>
#include <vector>
#include "Enums.h"
#include "SeatingPlan.h"

class Ticket {
private:
    int id;
    int ownerId;
    int eventId;
    std::string eventTitle;
    int count;
    double totalPrice;
    std::vector<Seat> seats;
    ParticipationType participationType;

public:
    Ticket();
    Ticket(int id, int ownerId, int eventId, const std::string& eventTitle, int count, double totalPrice, const std::vector<Seat>& seats, ParticipationType participationType);

    int getId() const;
    int getOwnerId() const;
    int getEventId() const;
    const std::string& getEventTitle() const;
    int getCount() const;
    double getTotalPrice() const;
    const std::vector<Seat>& getSeats() const;
    ParticipationType getParticipationType() const;

    void print() const;
    std::vector<std::string> toRecord() const;
};

#endif
