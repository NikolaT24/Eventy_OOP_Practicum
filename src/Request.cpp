#include "Request.h"
#include <iostream>

Request::Request() {
    this->id = 0;
    this->eventId = 0;
    this->creatorId = 0;
    this->eventTitle = "";
    this->status = RequestStatus::Pending;
    this->rejectReason = "";
}

Request::Request(
    int id,
    int eventId,
    int creatorId,
    const std::string& eventTitle
) {
    this->id = id;
    this->eventId = eventId;
    this->creatorId = creatorId;
    this->eventTitle = eventTitle;
    this->status = RequestStatus::Pending;
    this->rejectReason = "";
}

int Request::getId() const {
    return this->id;
}

int Request::getEventId() const {
    return this->eventId;
}

int Request::getCreatorId() const {
    return this->creatorId;
}

std::string Request::getEventTitle() const {
    return this->eventTitle;
}

RequestStatus Request::getStatus() const {
    return this->status;
}

std::string Request::getStatusAsText() const {
    if (this->status == RequestStatus::Pending) {
        return "Pending";
    }
    if (this->status == RequestStatus::Approved) {
        return "Approved";
    }

    return "Rejected";
}

std::string Request::getRejectReason() const {
    return this->rejectReason;
}

bool Request::isPending() const {
    return this->status == RequestStatus::Pending;
}

void Request::approve() {
    if (this->status == RequestStatus::Pending) {
        this->status = RequestStatus::Approved;
    }
}

void Request::reject(const std::string& reason) {
    if (this->status == RequestStatus::Pending) {
        this->status = RequestStatus::Rejected;
        this->rejectReason = reason;
    }
}

void Request::print() const {
    std::cout << "Request #" << this->id << std::endl;
    std::cout << "Event id: " << this->eventId << std::endl;
    std::cout << "Event title: " << this->eventTitle << std::endl;
    std::cout << "Creator id: " << this->creatorId << std::endl;
    std::cout << "Status: " << this->getStatusAsText() << std::endl;

    if (this->status == RequestStatus::Rejected) {
        std::cout << "Reason: " << this->rejectReason << std::endl;
    }
}
