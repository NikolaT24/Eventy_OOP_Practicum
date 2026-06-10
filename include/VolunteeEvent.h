#ifndef VOLUNTEER_EVENT_H
#define VOLUNTEER_EVENT_H

#include "Event.h"
#include <vector>

class VolunteerEvent : public Event {
private:
    std::string description;
    bool applicationsOpen;
    std::vector<int> participantIds;

public:
    VolunteerEvent();
    VolunteerEvent(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, const std::string& description);
    VolunteerEvent(int id, const std::string& title, const std::string& date, const std::string& address, int creatorId, EventStatus status, const std::string& description, bool applicationsOpen, const std::vector<int>& participantIds);

    EventType getType() const override;
    const std::string& getDescription() const;
    bool areApplicationsOpen() const;
    const std::vector<int>& getParticipantIds() const;

    bool hasParticipant(int clientId) const;
    bool addParticipant(int clientId);
    void closeApplications();

    void printInfo() const override;
    std::vector<std::string> toRecord() const override;
};

#endif
