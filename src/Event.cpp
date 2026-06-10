#include "Event.h"
#include <iostream>

Event::Event() {
    this->id = 0;
    this->title = "";
    this->date = "";
    this->address = "";
    this->creatorId = 0;
    this->status = EventStatus::Pending;
}

Event::Event(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, EventStatus status) {
    this->id = id;
    this->title = title;
    this->date = date;
    this->address = address;
    this->creatorId = creatorId;
    this->status = status;
}

int Event::getId() const {
    return this->id;
}

const std::string& Event::getTitle() const {
    return this->title;
}

const std::string& Event::getDate() const {
    return this->date;
}

const std::string& Event::getAddress() const {
    return this->address;
}

int Event::getCreatorId() const {
    return this->creatorId;
}

EventStatus Event::getStatus() const {
    return this->status;
}

bool Event::isPending() const {
    return this->status == EventStatus::Pending;
}

bool Event::isPublished() const {
    return this->status == EventStatus::Published;
}

bool Event::isCancelled() const {
    return this->status == EventStatus::Cancelled;
}

void Event::publish() {
    if (this->status == EventStatus::Pending) {
        this->status = EventStatus::Published;
    }
}

void Event::cancel() {
    this->status = EventStatus::Cancelled;
}

void Event::printShort() const {
    std::cout << "[" << this->id << "] "
              << this->title << " | "
              << this->date << " | "
              << this->address << " | "
              << toString(this->getType()) << " | "
              << toString(this->status) << std::endl;
}
