#pragma once

#include <expected>
#include <memory>
#include <string>
#include <vector>
#include "Event.h"
#include "Request.h"
#include "Ticket.h"
#include "User.h"

class EntityFactory {
public:
    static std::expected<std::unique_ptr<User>, std::string>
    userFromRecord(const std::vector<std::string>& parts);

    static std::expected<std::unique_ptr<Event>, std::string>
    eventFromRecord(const std::vector<std::string>& parts);

    static std::expected<std::unique_ptr<Request>, std::string>
    requestFromRecord(const std::vector<std::string>& parts);

    static std::expected<Ticket, std::string>
    ticketFromRecord(const std::vector<std::string>& parts);
};
