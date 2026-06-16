#include "Ticket.h"
#include "EventyException.h"

Ticket::Ticket(int id, int ownerId, int eventId, std::string eventTitle,
               int count, double totalPrice, std::vector<Seat> seats)
    : id(id),
      ownerId(ownerId),
      eventId(eventId),
      eventTitle(std::move(eventTitle)),
      count(count),
      totalPrice(totalPrice),
      seats(std::move(seats)) {
    if (id <= 0 || ownerId <= 0 || eventId <= 0 || count <= 0 || totalPrice < 0) {
        throw ValidationException("Ticket data is invalid.");
    }

    if (!this->seats.empty() && static_cast<int>(this->seats.size()) != count) {
        throw ValidationException("Seat count does not match ticket count.");
    }
}

int Ticket::getId() const {
    return id;
}

int Ticket::getOwnerId() const {
    return ownerId;
}

int Ticket::getEventId() const {
    return eventId;
}

const std::string& Ticket::getEventTitle() const {
    return eventTitle;
}

int Ticket::getCount() const {
    return count;
}

double Ticket::getTotalPrice() const {
    return totalPrice;
}

const std::vector<Seat>& Ticket::getSeats() const {
    return seats;
}

std::vector<std::string> Ticket::toRecord() const {
    return {
        "TICKET",
        std::to_string(id),
        std::to_string(ownerId),
        std::to_string(eventId),
        eventTitle,
        std::to_string(count),
        std::to_string(totalPrice),
        SeatingPlan::encodeSeats(seats)
    };
}

std::ostream& operator<<(std::ostream& output, const Ticket& ticket) {
    output << "Ticket #" << ticket.id
           << " | " << ticket.eventTitle
           << " | count: " << ticket.count
           << " | paid: " << ticket.totalPrice;

    if (!ticket.seats.empty()) {
        output << " | seats: " << SeatingPlan::encodeSeats(ticket.seats);
    }

    return output;
}
