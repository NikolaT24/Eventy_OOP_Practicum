#include "Ticket.h"
#include <iostream>

Ticket::Ticket() {
    this->id = 0;
    this->ownerId = 0;
    this->eventId = 0;
    this->eventTitle = "";
    this->count = 0;
    this->totalPrice = 0;
    this->participationType = ParticipationType::Ticket;
}

Ticket::Ticket(int id, int ownerId, int eventId, const std::string& eventTitle, int count, double totalPrice, const std::vector<Seat>& seats, ParticipationType participationType) {
    this->id = id;
    this->ownerId = ownerId;
    this->eventId = eventId;
    this->eventTitle = eventTitle;
    this->count = count;
    this->totalPrice = totalPrice;
    this->seats = seats;
    this->participationType = participationType;
}

int Ticket::getId() const {
    return this->id;
}

int Ticket::getOwnerId() const {
    return this->ownerId;
}

int Ticket::getEventId() const {
    return this->eventId;
}

const std::string& Ticket::getEventTitle() const {
    return this->eventTitle;
}

int Ticket::getCount() const {
    return this->count;
}

double Ticket::getTotalPrice() const {
    return this->totalPrice;
}

const std::vector<Seat>& Ticket::getSeats() const {
    return this->seats;
}

ParticipationType Ticket::getParticipationType() const {
    return this->participationType;
}

void Ticket::print() const {
    std::cout << "Record #" << this->id << std::endl;
    std::cout << "Type: " << toString(this->participationType) << std::endl;
    std::cout << "Event: " << this->eventTitle << " (#" << this->eventId << ")" << std::endl;
    std::cout << "Count: " << this->count << std::endl;
    std::cout << "Total price: " << this->totalPrice << std::endl;

    if (!this->seats.empty()) {
        std::cout << "Seats: ";

        for (int i = 0; i < (int)this->seats.size(); i++) {
            if (i > 0) {
                std::cout << ", ";
            }

            std::cout << "(" << this->seats[i].first << "," << this->seats[i].second << ")";
        }

        std::cout << std::endl;
    }
}

std::vector<std::string> Ticket::toRecord() const {
    return {
        "TICKET",
        std::to_string(this->id),
        std::to_string(this->ownerId),
        std::to_string(this->eventId),
        this->eventTitle,
        std::to_string(this->count),
        std::to_string(this->totalPrice),
        SeatingPlan::encodeSeats(this->seats),
        toString(this->participationType)
    };
}
