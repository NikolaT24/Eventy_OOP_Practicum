#pragma once

#include <memory>
#include <vector>
#include "Event.h"
#include "SeatingPlan.h"

class TicketedEvent final : public Event {
private:
    double ticketPrice;
    SeatingPlan seatingPlan;

public:
    TicketedEvent(int id, std::string title, std::string date, std::string address,
                  int creatorId, double ticketPrice, SeatingPlan seatingPlan,
                  EventStatus status = EventStatus::Pending,
                  std::string cancellationReason = "");

    double getTicketPrice() const;
    const SeatingPlan& getSeatingPlan() const;
    SeatingPlan& getSeatingPlan();

    double priceFor(int count) const;
    void reserveGeneral(int count);
    void reserveSeats(const std::vector<Seat>& seats);

    void accept(EventVisitor& visitor) const override;
    std::unique_ptr<Event> clone() const override;
};
