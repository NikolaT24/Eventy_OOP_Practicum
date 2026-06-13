#pragma once

#include <memory>
#include <ostream>
#include <string>
#include "Enums.h"

class EventVisitor;

class Event {
private:
    int id;
    std::string title;
    std::string date;
    std::string address;
    int creatorId;
    EventStatus status;
    std::string cancellationReason;

protected:
    Event(int id, std::string title, std::string date, std::string address, int creatorId, EventStatus status = EventStatus::Pending, std::string cancellationReason = "");

public:
    virtual ~Event() = default;

    Event(const Event&) = default;
    Event& operator=(const Event&) = default;
    Event(Event&&) noexcept = default;
    Event& operator=(Event&&) noexcept = default;

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getDate() const;
    const std::string& getAddress() const;
    int getCreatorId() const;
    EventStatus getStatus() const;
    const std::string& getCancellationReason() const;

    bool isPending() const;
    bool isPublished() const;
    bool isCancelled() const;

    void publish();
    void cancel(const std::string& reason);

    void printSummary(std::ostream& output) const;

    virtual void accept(EventVisitor& visitor) const = 0;
    virtual std::unique_ptr<Event> clone() const = 0;
};
