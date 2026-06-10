#include "VolunteerEvent.h"
#include "StringUtils.h"
#include <iostream>

VolunteerEvent::VolunteerEvent() : Event() {
    this->description = "";
    this->applicationsOpen = true;
}

VolunteerEvent::VolunteerEvent(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, const std::string& description)
    : Event(id, title, date, address, creatorId, EventStatus::Pending) {
    this->description = description;
    this->applicationsOpen = true;
}

VolunteerEvent::VolunteerEvent(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, EventStatus status, const std::string& description, bool applicationsOpen, const std::vector<int>& participantIds)
    : Event(id, title, date, address, creatorId, status) {
    this->description = description;
    this->applicationsOpen = applicationsOpen;
    this->participantIds = participantIds;
}

EventType VolunteerEvent::getType() const {
    return EventType::Volunteer;
}

const std::string& VolunteerEvent::getDescription() const {
    return this->description;
}

bool VolunteerEvent::areApplicationsOpen() const {
    return this->applicationsOpen;
}

const std::vector<int>& VolunteerEvent::getParticipantIds() const {
    return this->participantIds;
}

bool VolunteerEvent::hasParticipant(int clientId) const {
    for (int participantId : this->participantIds) {
        if (participantId == clientId) {
            return true;
        }
    }

    return false;
}

bool VolunteerEvent::addParticipant(int clientId) {
    if (this->hasParticipant(clientId)) {
        return false;
    }

    this->participantIds.push_back(clientId);
    return true;
}

void VolunteerEvent::closeApplications() {
    this->applicationsOpen = false;
}

void VolunteerEvent::printInfo() const {
    std::cout << "Id: " << this->id << std::endl;
    std::cout << "Title: " << this->title << std::endl;
    std::cout << "Date: " << this->date << std::endl;
    std::cout << "Address: " << this->address << std::endl;
    std::cout << "Type: Volunteer" << std::endl;
    std::cout << "Status: " << toString(this->status) << std::endl;
    std::cout << "Description: " << this->description << std::endl;
    std::cout << "Applications: " << (this->applicationsOpen ? "Open" : "Closed") << std::endl;
    std::cout << "Participants: " << this->participantIds.size() << std::endl;
}

std::vector<std::string> VolunteerEvent::toRecord() const {
    std::vector<std::string> participantParts;

    for (int participantId : this->participantIds) {
        participantParts.push_back(std::to_string(participantId));
    }

    return {
        "EVENT",
        "Volunteer",
        std::to_string(this->id),
        this->title,
        this->date,
        this->address,
        std::to_string(this->creatorId),
        toString(this->status),
        this->description,
        this->applicationsOpen ? "1" : "0",
        StringUtils::joinEscaped(participantParts, ',')
    };
}
