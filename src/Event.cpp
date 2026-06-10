#include "Event.h"

Event::Event() {
    this->id = 0;
    this->title = "";
    this->date = "";
    this->address = "";
    this->creatorId = 0;

    this->type = EventType::Ticketed;
    this->status = EventStatus::Pending;

    this->ticketPrice = 0;
    this->capacity = 0;
    this->soldTickets = 0;

    this->volunteerDescription = "";
}

Event::Event(
    int id,
    const std::string& title,
    const std::string& date,
    const std::string& address,
    int creatorId,
    EventType type
) {
    this->id = id;
    this->title = title;
    this->date = date;
    this->address = address;
    this->creatorId = creatorId;

    this->type = type;
    this->status = EventStatus::Pending;

    this->ticketPrice = 0;
    this->capacity = 0;
    this->soldTickets = 0;

    this->volunteerDescription = "";
}

int Event::getId() const {
    return this->id;
}

std::string Event::getTitle() const {
    return this->title;
}

std::string Event::getDate() const {
    return this->date;
}

std::string Event::getAddress() const {
    return this->address;
}

int Event::getCreatorId() const {
    return this->creatorId;
}

EventType Event::getType() const {
    return this->type;
}

EventStatus Event::getStatus() const {
    return this->status;
}

std::string Event::getTypeAsText() const {
    if (this->type == EventType::Ticketed) {
        return "Ticketed";
    }
    return "Volunteer";
}

std::string Event::getStatusAsText() const {
    if (this->status == EventStatus::Pending) {
        return "Pending";
    }
    if (this->status == EventStatus::Published) {
        return "Published";
    }
    return "Cancelled";
}

void Event::publish() {
    if (this->status == EventStatus::Pending) {
        this->status = EventStatus::Published;
    }
}

void Event::cancel() {
    if (this->status != EventStatus::Cancelled) {
        this->status = EventStatus::Cancelled;
    }
}

bool Event::isPublished() const {
    return this->status == EventStatus::Published;
}

bool Event::isTicketed() const {
    return this->type == EventType::Ticketed;
}

bool Event::isVolunteer() const {
    return this->type == EventType::Volunteer;
}

void Event::setTicketInfo(double price, int capacity) {
    if (this->type == EventType::Ticketed && price >= 0 && capacity > 0) {
        this->ticketPrice = price;
        this->capacity = capacity;
    }
}

double Event::getTicketPrice() const {
    return this->ticketPrice;
}

int Event::getCapacity() const {
    return this->capacity;
}

int Event::getSoldTickets() const {
    return this->soldTickets;
}

int Event::getAvailableTickets() const {
    return this->capacity - this->soldTickets;
}

bool Event::canSellTickets(int count) const {
    if (this->type != EventType::Ticketed) {
        return false;
    }
    if (this->status != EventStatus::Published) {
        return false;
    }
    if (count <= 0) {
        return false;
    }
    return this->soldTickets + count <= this->capacity;
}

bool Event::sellTickets(int count) {
    if (!this->canSellTickets(count)) {
        return false;
    }
    this->soldTickets += count;
    return true;
}

void Event::setVolunteerDescription(const std::string& description) {
    if (this->type == EventType::Volunteer) {
        this->volunteerDescription = description;
    }
}

std::string Event::getVolunteerDescription() const {
    return this->volunteerDescription;
}
