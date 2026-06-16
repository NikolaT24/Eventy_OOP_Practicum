#pragma once

#include <expected>
#include <string>
#include <vector>
#include "Repositories.h"

struct IdState {
    int nextUserId = 1;
    int nextEventId = 1;
    int nextTicketId = 1;
    int nextRequestId = 1;
    int nextNotificationId = 1;
};

class StorageService {
private:
    std::string filePath;

    std::expected<void, std::string> loadRecord(
        const std::vector<std::string>& parts,
        UserRepository& users,
        EventRepository& events,
        RequestRepository& requests,
        TicketRepository& tickets,
        IdState& ids
    ) const;

public:
    explicit StorageService(std::string filePath);

    std::expected<void, std::string> load(
        UserRepository& users,
        EventRepository& events,
        RequestRepository& requests,
        TicketRepository& tickets,
        IdState& ids
    ) const;

    std::expected<void, std::string> save(
        const UserRepository& users,
        const EventRepository& events,
        const RequestRepository& requests,
        const TicketRepository& tickets,
        const IdState& ids
    ) const;
};
