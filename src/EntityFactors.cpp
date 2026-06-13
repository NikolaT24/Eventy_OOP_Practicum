#include "EntityFactory.h"
#include "Admin.h"
#include "Client.h"
#include "TicketedEvent.h"
#include "Utils.h"
#include "VolunteerEvent.h"

namespace {
    std::vector<int> parseIds(const std::string& value) {
        std::vector<int> result;
        if (value.empty()) 
          return result;

        for (const std::string& part : utils::split(value, ',')) {
            auto id = utils::toInt(part);
            if (id) 
              result.push_back(*id);
        }

        return result;
    }
}

std::expected<std::unique_ptr<User>, std::string>
EntityFactory::userFromRecord(const std::vector<std::string>& parts) {
    try {
        if (parts.size() < 6 || parts[0] != "USER")
            return std::unexpected("Invalid user record.");
          
        auto id = utils::toInt(parts[2]);
        if (!id) return std::unexpected(id.error());

        if (parts[1] == "CLIENT") {
            if (parts.size() != 9) return std::unexpected("Invalid client record.");

            auto balance = utils::toDouble(parts[5]);
            if (!balance) return std::unexpected(balance.error());

            return std::unique_ptr<User>(std::make_unique<Client>(
                *id, parts[3], parts[4], *balance,
                parseIds(parts[6]), parseIds(parts[7]), parseIds(parts[8])
            ));
        }

        if (parts[1] == "ADMIN") {
            if (parts.size() != 6) return std::unexpected("Invalid admin record.");

            return std::unique_ptr<User>(std::make_unique<Admin>(
                *id, parts[3], parts[4], parseIds(parts[5])
            ));
        }

        return std::unexpected("Unknown user type: " + parts[1]);
    } catch (const std::exception& error) {
        return std::unexpected(error.what());
    }
}

std::expected<std::unique_ptr<Event>, std::string>
EntityFactory::eventFromRecord(const std::vector<std::string>& parts) {
    try {
        if (parts.size() < 10 || parts[0] != "EVENT")
            return std::unexpected("Invalid event record.");

        auto id = utils::toInt(parts[2]);
        auto creatorId = utils::toInt(parts[6]);
        auto status = eventStatusFromString(parts[7]);

        if (!id) 
          return std::unexpected(id.error());
        if (!creatorId) 
          return std::unexpected(creatorId.error());
        if (!status) 
          return std::unexpected(status.error());

        if (parts[1] == "TICKETED") {
            if (parts.size() != 16) return std::unexpected("Invalid ticketed event record.");

            auto price = utils::toDouble(parts[9]);
            auto mode = seatingModeFromString(parts[10]);
            auto capacity = utils::toInt(parts[11]);
            auto rows = utils::toInt(parts[12]);
            auto columns = utils::toInt(parts[13]);
            auto sold = utils::toInt(parts[14]);

            if (!price) 
              return std::unexpected(price.error());
            if (!mode) 
              return std::unexpected(mode.error());
            if (!capacity) 
              return std::unexpected(capacity.error());
            if (!rows) 
              return std::unexpected(rows.error());
            if (!columns) 
              return std::unexpected(columns.error());
            if (!sold) 
              return std::unexpected(sold.error());

            SeatingPlan seating = SeatingPlan::restore(
                *mode, *capacity, *rows, *columns, *sold,
                SeatingPlan::decodeSeats(parts[15])
            );

            return std::unique_ptr<Event>(std::make_unique<TicketedEvent>(
                *id, parts[3], parts[4], parts[5], *creatorId,
                *price, std::move(seating), *status, parts[8]
            ));
        }

        if (parts[1] == "VOLUNTEER") {
            if (parts.size() != 12) return std::unexpected("Invalid volunteer event record.");

            return std::unique_ptr<Event>(std::make_unique<VolunteerEvent>(
                *id, parts[3], parts[4], parts[5], *creatorId,
                parts[9], *status, parts[10] == "1", parseIds(parts[11]), parts[8]
            ));
        }

        return std::unexpected("Unknown event type: " + parts[1]);
    } catch (const std::exception& error) {
        return std::unexpected(error.what());
    }
}

std::expected<std::unique_ptr<Request>, std::string>
EntityFactory::requestFromRecord(const std::vector<std::string>& parts) {
    try {
        if (parts.size() < 7 || parts[0] != "REQUEST")
            return std::unexpected("Invalid request record.");

        auto id = utils::toInt(parts[2]);
        auto requesterId = utils::toInt(parts[3]);
        auto eventId = utils::toInt(parts[4]);
        auto status = requestStatusFromString(parts[5]);

        if (!id) 
          return std::unexpected(id.error());
        if (!requesterId) 
          return std::unexpected(requesterId.error());
        if (!eventId) 
          return std::unexpected(eventId.error());
        if (!status) 
          return std::unexpected(status.error());

        if (parts[1] == "PUBLISH") {
            if (parts.size() != 7) return std::unexpected("Invalid publish request record.");

            return std::unique_ptr<Request>(std::make_unique<PublishEventRequest>(
                *id, *requesterId, *eventId, *status, parts[6]
            ));
        }

        if (parts[1] == "VOLUNTEER") {
            if (parts.size() != 8) return std::unexpected("Invalid volunteer request record.");

            return std::unique_ptr<Request>(std::make_unique<VolunteerApplicationRequest>(
                *id, *requesterId, *eventId, parts[7], *status, parts[6]
            ));
        }

        return std::unexpected("Unknown request type: " + parts[1]);
    } catch (const std::exception& error) {
        return std::unexpected(error.what());
    }
}

std::expected<Ticket, std::string>
EntityFactory::ticketFromRecord(const std::vector<std::string>& parts) {
    try {
        if (parts.size() != 8 || parts[0] != "TICKET")
            return std::unexpected("Invalid ticket record.");

        auto id = utils::toInt(parts[1]);
        auto ownerId = utils::toInt(parts[2]);
        auto eventId = utils::toInt(parts[3]);
        auto count = utils::toInt(parts[5]);
        auto totalPrice = utils::toDouble(parts[6]);

        if (!id) return std::unexpected(id.error());
        if (!ownerId) return std::unexpected(ownerId.error());
        if (!eventId) return std::unexpected(eventId.error());
        if (!count) return std::unexpected(count.error());
        if (!totalPrice) return std::unexpected(totalPrice.error());

        return Ticket(*id, *ownerId, *eventId, parts[4], *count, *totalPrice,
                      SeatingPlan::decodeSeats(parts[7]));
    } catch (const std::exception& error) {
        return std::unexpected(error.what());
    }
}
