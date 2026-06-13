#pragma once

#include <ostream>
#include <string>
#include <vector>
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

public:
    Ticket(int id, int ownerId, int eventId, std::string eventTitle, int count, double totalPrice, std::vector<Seat> seats = {});

    int getId() const;
    int getOwnerId() const;
    int getEventId() const;
    const std::string& getEventTitle() const;
    int getCount() const;
    double getTotalPrice() const;
    const std::vector<Seat>& getSeats() const;
    std::vector<std::string> toRecord() const;

    friend std::ostream& operator<<(std::ostream& output, const Ticket& ticket);
};
