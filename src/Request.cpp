#include "Request.h"
#include <iostream>

Request::Request() {
    this->id = 0;
    this->type = RequestType::PublishEvent;
    this->status = RequestStatus::Pending;
    this->requesterId = 0;
    this->eventId = 0;
    this->text = "";
    this->reason = "";
}

Request::Request(int id, RequestType type, int requesterId, int eventId, const std::string& text) {
    this->id = id;
    this->type = type;
    this->status = RequestStatus::Pending;
    this->requesterId = requesterId;
    this->eventId = eventId;
    this->text = text;
    this->reason = "";
}

Request::Request(int id, RequestType type, RequestStatus status, int requesterId, int eventId, const std::string& text, const std::string& reason) {
    this->id = id;
    this->type = type;
    this->status = status;
    this->requesterId = requesterId;
    this->eventId = eventId;
    this->text = text;
    this->reason = reason;
}

int Request::getId() const {
    return this->id;
}

RequestType Request::getType() const {
    return this->type;
}

RequestStatus Request::getStatus() const {
    return this->status;
}

int Request::getRequesterId() const {
    return this->requesterId;
}

int Request::getEventId() const {
    return this->eventId;
}

const std::string& Request::getText() const {
    return this->text;
}

const std::string& Request::getReason() const {
    return this->reason;
}

bool Request::isPending() const {
    return this->status == RequestStatus::Pending;
}

bool Request::isPublishRequest() const {
    return this->type == RequestType::PublishEvent;
}

bool Request::isVolunteerApplication() const {
    return this->type == RequestType::VolunteerApplication;
}

void Request::approve() {
    if (this->status == RequestStatus::Pending) {
        this->status = RequestStatus::Approved;
    }
}

void Request::reject(const std::string& reason) {
    if (this->status == RequestStatus::Pending) {
        this->status = RequestStatus::Rejected;
        this->reason = reason;
    }
}

void Request::print() const {
    std::cout << "Request #" << this->id << std::endl;
    std::cout << "Type: " << toString(this->type) << std::endl;
    std::cout << "Status: " << toString(this->status) << std::endl;
    std::cout << "Requester id: " << this->requesterId << std::endl;
    std::cout << "Event id: " << this->eventId << std::endl;
    std::cout << "Text: " << this->text << std::endl;

    if (!this->reason.empty()) {
        std::cout << "Reason: " << this->reason << std::endl;
    }
}

std::vector<std::string> Request::toRecord() const {
    return {
        "REQUEST",
        std::to_string(this->id),
        toString(this->type),
        toString(this->status),
        std::to_string(this->requesterId),
        std::to_string(this->eventId),
        this->text,
        this->reason
    };
}
