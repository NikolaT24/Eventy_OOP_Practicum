#include "TicketedEvent.h"
#include <iostream>

TicketedEvent::TicketedEvent() : Event() {
    this->price = 0;
    this->seating = SeatingPlan::general(0);
}

TicketedEvent::TicketedEvent(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, double price, const SeatingPlan& seating)
    : Event(id, title, date, address, creatorId, EventStatus::Pending) {
    this->price = price;
    this->seating = seating;
}

TicketedEvent::TicketedEvent(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, EventStatus status, double price, const SeatingPlan& seating)
    : Event(id, title, date, address, creatorId, status) {
    this->price = price;
    this->seating = seating;
}

EventType TicketedEvent::getType() const {
    return EventType::Ticketed;
}

double TicketedEvent::getPrice() const {
    return this->price;
}

const SeatingPlan& TicketedEvent::getSeating() const {
    return this->seating;
}

SeatingPlan& TicketedEvent::getSeating() {
    return this->seating;
}

bool TicketedEvent::canBuyGeneral(int count) const {
    return this->isPublished() && this->seating.canReserveGeneral(count);
}

bool TicketedEvent::canBuySeats(const std::vector<Seat>& seats) const {
    return this->isPublished() && this->seating.canReserveSeats(seats);
}

bool TicketedEvent::buyGeneral(int count) {
    if (!this->canBuyGeneral(count)) {
        return false;
    }

    return this->seating.reserveGeneral(count);
}

bool TicketedEvent::buySeats(const std::vector<Seat>& seats) {
    if (!this->canBuySeats(seats)) {
        return false;
    }

    return this->seating.reserveSeats(seats);
}

void TicketedEvent::printInfo() const {
    std::cout << "Id: " << this->id << std::endl;
    std::cout << "Title: " << this->title << std::endl;
    std::cout << "Date: " << this->date << std::endl;
    std::cout << "Address: " << this->address << std::endl;
    std::cout << "Type: Ticketed" << std::endl;
    std::cout << "Status: " << toString(this->status) << std::endl;
    std::cout << "Price: " << this->price << std::endl;
    std::cout << "Seating: " << toString(this->seating.getMode()) << std::endl;
    std::cout << "Capacity: " << this->seating.getCapacity() << std::endl;
    std::cout << "Available: " << this->seating.getAvailableCount() << std::endl;
}

std::vector<std::string> TicketedEvent::toRecord() const {
    return {
        "EVENT",
        "Ticketed",
        std::to_string(this->id),
        this->title,
        this->date,
        this->address,
        std::to_string(this->creatorId),
        toString(this->status),
        std::to_string(this->price),
        toString(this->seating.getMode()),
        std::to_string(this->seating.getCapacity()),
        std::to_string(this->seating.getRows()),
        std::to_string(this->seating.getCols()),
        std::to_string(this->seating.getSoldCount()),
        SeatingPlan::encodeSeats(this->seating.getOccupiedSeats())
    };
}
