#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include "Enums.h"

class Event {
protected:
    int id;
    std::string title;
    std::string date;
    std::string address;
    int creatorId;
    EventStatus status;

public:
    Event();
    Event(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, EventStatus status = EventStatus::Pending);
    virtual ~Event() = default;

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getDate() const;
    const std::string& getAddress() const;
    int getCreatorId() const;
    EventStatus getStatus() const;

    bool isPending() const;
    bool isPublished() const;
    bool isCancelled() const;

    void publish();
    void cancel();

    virtual EventType getType() const = 0;
    virtual void printInfo() const = 0;
    virtual void printShort() const;
    virtual std::vector<std::string> toRecord() const = 0;
};

#endif
