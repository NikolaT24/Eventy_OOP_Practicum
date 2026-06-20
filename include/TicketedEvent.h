#pragma once

#include "Event.h"
#include "SeatingPlan.h"
#include <vector>

class TicketedEvent : public Event {
private:
    double ticketPrice;
    SeatingPlan seatingPlan;

public:
    TicketedEvent(int id,
                  const std::string& title,
                  const std::string& date,
                  const std::string& address,
                  int creatorId,
                  double ticketPrice,
                  const SeatingPlan& seatingPlan,
                  EventStatus status = EventStatus::Pending);

    EventType getType() const override;

    double getTicketPrice() const;
    const SeatingPlan& getSeatingPlan() const;
    SeatingPlan& getSeatingPlan();

    double priceFor(int count) const;
    bool reserveGeneral(int count);
    bool reserveSeats(const std::vector<Seat>& seats);

    void printInfo() const override;
    std::vector<std::string> toRecord() const override;
};