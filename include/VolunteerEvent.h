#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Event.h"

class VolunteerEvent final : public Event {
private:
    std::string description;
    bool applicationsOpen;
    std::vector<int> participantIds;

public:
    VolunteerEvent(int id, std::string title, std::string date, std::string address,
                   int creatorId, std::string description,
                   EventStatus status = EventStatus::Pending,
                   bool applicationsOpen = true,
                   std::vector<int> participantIds = {},
                   std::string cancellationReason = "");

    const std::string& getDescription() const;
    bool areApplicationsOpen() const;
    const std::vector<int>& getParticipantIds() const;

    bool hasParticipant(int clientId) const;
    void addParticipant(int clientId);
    void closeApplications();

    void accept(EventVisitor& visitor) const override;
    std::unique_ptr<Event> clone() const override;
};