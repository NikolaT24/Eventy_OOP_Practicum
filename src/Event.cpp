#include "Event.h"

#include "EventyException.h"
#include "Utils.h"

Event::Event(int id, std::string title, std::string date, std::string address,
             int creatorId, EventStatus status, std::string cancellationReason)
    : id(id),
      title(std::move(title)),
      date(std::move(date)),
      address(std::move(address)),
      creatorId(creatorId),
      status(status),
      cancellationReason(std::move(cancellationReason)) {
    if (id <= 0 || creatorId <= 0) {
        throw ValidationException("Event and creator ids must be positive.");
    }

    if (this->title.empty() || this->address.empty() || !utils::isValidDate(this->date)) {
        throw ValidationException("Event title, date, or address is invalid.");
    }
}

int Event::getId() const {
    return id;
}

const std::string& Event::getTitle() const {
    return title;
}

const std::string& Event::getDate() const {
    return date;
}

const std::string& Event::getAddress() const {
    return address;
}

int Event::getCreatorId() const {
    return creatorId;
}

EventStatus Event::getStatus() const {
    return status;
}

const std::string& Event::getCancellationReason() const {
    return cancellationReason;
}

bool Event::isPending() const {
    return status == EventStatus::Pending;
}

bool Event::isPublished() const {
    return status == EventStatus::Published;
}

bool Event::isCancelled() const {
    return status == EventStatus::Cancelled;
}

void Event::publish() {
    if (!isPending()) {
        throw InvalidStateException("Only a pending event can be published.");
    }

    status = EventStatus::Published;
}

void Event::cancel(const std::string& reason) {
    if (isCancelled()) {
        throw InvalidStateException("The event is already cancelled.");
    }

    if (utils::trim(reason).empty()) {
        throw ValidationException("A cancellation reason is required.");
    }

    status = EventStatus::Cancelled;
    cancellationReason = utils::trim(reason);
}

void Event::printSummary(std::ostream& output) const {
    output << '[' << id << "] " << title
           << " | " << date
           << " | " << address
           << " | " << toString(status);
}
