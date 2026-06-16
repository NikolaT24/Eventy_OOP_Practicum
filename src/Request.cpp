#include "Request.h"
#include "EventyException.h"
#include "Utils.h"

Request::Request(int id, int requesterId, int eventId,
                 RequestStatus status, std::string rejectionReason)
    : id(id),
      requesterId(requesterId),
      eventId(eventId),
      status(status),
      rejectionReason(std::move(rejectionReason)) {
    if (id <= 0 || requesterId <= 0 || eventId <= 0) {
        throw ValidationException("Request ids must be positive.");
    }
}

std::vector<std::string> Request::baseRecord(const std::string& type) const {
    return {
        "REQUEST",
        type,
        std::to_string(id),
        std::to_string(requesterId),
        std::to_string(eventId),
        toString(status),
        rejectionReason
    };
}

int Request::getId() const {
    return id;
}

int Request::getRequesterId() const {
    return requesterId;
}

int Request::getEventId() const {
    return eventId;
}

RequestStatus Request::getStatus() const {
    return status;
}

const std::string& Request::getRejectionReason() const {
    return rejectionReason;
}

bool Request::isPending() const {
    return status == RequestStatus::Pending;
}

void Request::approve() {
    if (!isPending()) {
        throw InvalidStateException("The request has already been processed.");
    }

    status = RequestStatus::Approved;
}

void Request::reject(const std::string& reason) {
    if (!isPending()) {
        throw InvalidStateException("The request has already been processed.");
    }

    if (utils::trim(reason).empty()) {
        throw ValidationException("A rejection reason is required.");
    }

    status = RequestStatus::Rejected;
    rejectionReason = utils::trim(reason);
}

PublishEventRequest::PublishEventRequest(int id, int requesterId, int eventId,
                                         RequestStatus status, std::string rejectionReason)
    : Request(id, requesterId, eventId, status, std::move(rejectionReason)) {}

bool PublishEventRequest::isPublishRequest() const {
    return true;
}

bool PublishEventRequest::isVolunteerApplication() const {
    return false;
}

std::string PublishEventRequest::summary() const {
    return "Publish event request #" + std::to_string(getId()) +
           " | event #" + std::to_string(getEventId()) +
           " | requester #" + std::to_string(getRequesterId()) +
           " | " + toString(getStatus());
}

std::vector<std::string> PublishEventRequest::toRecord() const {
    return baseRecord("PUBLISH");
}

std::unique_ptr<Request> PublishEventRequest::clone() const {
    return std::make_unique<PublishEventRequest>(*this);
}

VolunteerApplicationRequest::VolunteerApplicationRequest(
    int id, int requesterId, int eventId, std::string motivation,
    RequestStatus status, std::string rejectionReason)
    : Request(id, requesterId, eventId, status, std::move(rejectionReason)),
      motivation(std::move(motivation)) {
    if (utils::trim(this->motivation).empty()) {
        throw ValidationException("Volunteer motivation is required.");
    }
}

const std::string& VolunteerApplicationRequest::getMotivation() const {
    return motivation;
}

bool VolunteerApplicationRequest::isPublishRequest() const {
    return false;
}

bool VolunteerApplicationRequest::isVolunteerApplication() const {
    return true;
}

std::string VolunteerApplicationRequest::summary() const {
    return "Volunteer application #" + std::to_string(getId()) +
           " | event #" + std::to_string(getEventId()) +
           " | requester #" + std::to_string(getRequesterId()) +
           " | " + toString(getStatus()) +
           " | motivation: " + motivation;
}

std::vector<std::string> VolunteerApplicationRequest::toRecord() const {
    std::vector<std::string> record = baseRecord("VOLUNTEER");
    record.push_back(motivation);
    return record;
}

std::unique_ptr<Request> VolunteerApplicationRequest::clone() const {
    return std::make_unique<VolunteerApplicationRequest>(*this);
}
