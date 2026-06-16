#include "VolunteerEvent.h"
#include <algorithm>
#include "EventVisitor.h"
#include "EventyException.h"
#include "Utils.h"

VolunteerEvent::VolunteerEvent(int id, std::string title, std::string date, std::string address,
                               int creatorId, std::string description,
                               EventStatus status, bool applicationsOpen,
                               std::vector<int> participantIds,
                               std::string cancellationReason)
    : Event(id, std::move(title), std::move(date), std::move(address), creatorId,
            status, std::move(cancellationReason)),
      description(std::move(description)),
      applicationsOpen(applicationsOpen),
      participantIds(std::move(participantIds)) {
    if (utils::trim(this->description).empty()) {
        throw ValidationException("Volunteer activity description is required.");
    }
}

const std::string& VolunteerEvent::getDescription() const {
    return description;
}

bool VolunteerEvent::areApplicationsOpen() const {
    return applicationsOpen;
}

const std::vector<int>& VolunteerEvent::getParticipantIds() const {
    return participantIds;
}

bool VolunteerEvent::hasParticipant(int clientId) const {
    return std::find(participantIds.begin(), participantIds.end(), clientId) != participantIds.end();
}

void VolunteerEvent::addParticipant(int clientId) {
    if (!isPublished() || isCancelled()) {
        throw InvalidStateException("Volunteers can be approved only for a published event.");
    }

    if (hasParticipant(clientId)) {
        throw InvalidStateException("The client is already a participant.");
    }

    participantIds.push_back(clientId);
}

void VolunteerEvent::closeApplications() {
    if (!isPublished() || isCancelled() || !utils::isUpcomingDate(getDate())) {
        throw InvalidStateException("Applications can be closed only for an upcoming published event.");
    }

    if (!applicationsOpen) {
        throw InvalidStateException("Volunteer applications are already closed.");
    }

    applicationsOpen = false;
}

void VolunteerEvent::accept(EventVisitor& visitor) const {
    visitor.visit(*this);
}

std::unique_ptr<Event> VolunteerEvent::clone() const {
    return std::make_unique<VolunteerEvent>(*this);
}
