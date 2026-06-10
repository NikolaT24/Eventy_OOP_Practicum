#include "Ticket.h"
#include <iostream>

Ticket::Ticket() {
    this->id = 0;
    this->ownerId = 0;
    this->eventId = 0;
    this->eventTitle = "";
    this->count = 0;
    this->totalPrice = 0;
}

Ticket::Ticket(
    int id,
    int ownerId,
    int eventId,
    const std::string& eventTitle,
    int count,
    double totalPrice
) {
    this->id = id;
    this->ownerId = ownerId;
    this->eventId = eventId;
    this->eventTitle = eventTitle;
    this->count = count;
    this->totalPrice = totalPrice;
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

std::string Ticket::getEventTitle() const {
    return this->eventTitle;
}

int Ticket::getCount() const {
    return this->count;
}

double Ticket::getTotalPrice() const {
    return this->totalPrice;
}

void Ticket::print() const {
    std::cout << "Ticket #" << this->id << std::endl;
    std::cout << "Event: " << this->eventTitle << std::endl;
    std::cout << "Event id: " << this->eventId << std::endl;
    std::cout << "Count: " << this->count << std::endl;
    std::cout << "Total price: " << this->totalPrice << std::endl;
}
