#include "TicketEvent.h"
#include "EventVisitor.h"
#include "EventyException.h"

TicketedEvent::TicketedEvent(int id, std::string title, std::string date, std::string address,
                             int creatorId, double ticketPrice, SeatingPlan seatingPlan,
                             EventStatus status, std::string cancellationReason)
    : Event(id, std::move(title), std::move(date), std::move(address), creatorId,
            status, std::move(cancellationReason)),
      ticketPrice(ticketPrice),
      seatingPlan(std::move(seatingPlan)) {
    if (ticketPrice < 0)
        throw ValidationException("Ticket price cannot be negative.");
}

double TicketedEvent::getTicketPrice() const {
    return ticketPrice;
}

const SeatingPlan& TicketedEvent::getSeatingPlan() const {
    return seatingPlan;
}

SeatingPlan& TicketedEvent::getSeatingPlan() {
    return seatingPlan;
}

double TicketedEvent::priceFor(int count) const {
    if (count <= 0)
        throw ValidationException("Ticket count must be positive.");

    return ticketPrice * count;
}

void TicketedEvent::reserveGeneral(int count) {
    if (!isPublished())
        throw InvalidStateException("Tickets can be purchased only for a published event.");

    seatingPlan.reserve(count);
}

void TicketedEvent::reserveSeats(const std::vector<Seat>& seats) {
    if (!isPublished())
        throw InvalidStateException("Tickets can be purchased only for a published event.");

    seatingPlan.reserve(seats);
}

void TicketedEvent::accept(EventVisitor& visitor) const {
    visitor.visit(*this);
}

std::unique_ptr<Event> TicketedEvent::clone() const {
    return std::make_unique<TicketedEvent>(*this);
}
