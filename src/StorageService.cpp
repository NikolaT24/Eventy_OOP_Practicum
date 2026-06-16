#include "StorageService.h"
#include <filesystem>
#include <fstream>
#include "Admin.h"
#include "Client.h"
#include "EntityFactory.h"
#include "EventVisitor.h"
#include "Notification.h"
#include "Utils.h"

namespace {
    std::string encodeIds(const std::vector<int>& ids) {
        std::vector<std::string> values;
        values.reserve(ids.size());

        for (int id : ids) values.push_back(std::to_string(id));
        return utils::join(values, ',');
    }
}

StorageService::StorageService(std::string filePath)
    : filePath(std::move(filePath)) {}

std::expected<void, std::string> StorageService::load(
    UserRepository& users,
    EventRepository& events,
    RequestRepository& requests,
    TicketRepository& tickets,
    IdState& ids
) const {
    std::ifstream input(filePath);

    if (!input.is_open()) {
        return {};
    }

    users.clear();
    events.clear();
    requests.clear();
    tickets.clear();

    std::string line;
    int lineNumber = 0;

    while (std::getline(input, line)) {
        ++lineNumber;
        if (utils::trim(line).empty()) continue;

        auto result = loadRecord(utils::splitEscaped(line, '|'), users, events, requests, tickets, ids);
        if (!result) {
            return std::unexpected("Line " + std::to_string(lineNumber) + ": " + result.error());
        }
    }

    return {};
}

std::expected<void, std::string> StorageService::save(
    const UserRepository& users,
    const EventRepository& events,
    const RequestRepository& requests,
    const TicketRepository& tickets,
    const IdState& ids
) const {
    try {
        std::filesystem::path path(filePath);
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }

        std::ofstream output(filePath, std::ios::trunc);
        if (!output.is_open()) {
            return std::unexpected("Cannot open storage file for writing.");
        }

        output << utils::joinEscaped({
            "META",
            std::to_string(ids.nextUserId),
            std::to_string(ids.nextEventId),
            std::to_string(ids.nextTicketId),
            std::to_string(ids.nextRequestId),
            std::to_string(ids.nextNotificationId)
        }, '|') << '\n';

        for (const auto& user : users.all()) {
            if (const auto* client = dynamic_cast<const Client*>(user.get())) {
                output << utils::joinEscaped({
                    "USER", "CLIENT",
                    std::to_string(client->getId()),
                    client->getUsername(),
                    client->getPassword(),
                    std::to_string(client->getBalance()),
                    encodeIds(client->getTicketIds()),
                    encodeIds(client->getHistoryEventIds()),
                    encodeIds(client->getCreatedEventIds())
                }, '|') << '\n';
            } else if (const auto* admin = dynamic_cast<const Admin*>(user.get())) {
                output << utils::joinEscaped({
                    "USER", "ADMIN",
                    std::to_string(admin->getId()),
                    admin->getUsername(),
                    admin->getPassword(),
                    encodeIds(admin->getProcessedRequestIds())
                }, '|') << '\n';
            }

            for (const Notification& notification : user->getNotifications()) {
                output << utils::joinEscaped({
                    "NOTIFICATION",
                    std::to_string(user->getId()),
                    std::to_string(notification.getId()),
                    notification.getCreatedAt(),
                    notification.isRead() ? "1" : "0",
                    notification.getMessage()
                }, '|') << '\n';
            }
        }

        for (const auto& event : events.all()) {
            EventRecordVisitor visitor;
            event->accept(visitor);
            output << utils::joinEscaped(visitor.getRecord(), '|') << '\n';
        }

        for (const auto& request : requests.all()) {
            output << utils::joinEscaped(request->toRecord(), '|') << '\n';
        }

        for (const Ticket& ticket : tickets.all()) {
            output << utils::joinEscaped(ticket.toRecord(), '|') << '\n';
        }

        return {};
    } catch (const std::exception& error) {
        return std::unexpected(error.what());
    }
}

std::expected<void, std::string> StorageService::loadRecord(
    const std::vector<std::string>& parts,
    UserRepository& users,
    EventRepository& events,
    RequestRepository& requests,
    TicketRepository& tickets,
    IdState& ids
) const {
    if (parts.empty()) return {};

    if (parts[0] == "META") {
        if (parts.size() != 6) return std::unexpected("Invalid metadata record.");

        auto userId = utils::toInt(parts[1]);
        auto eventId = utils::toInt(parts[2]);
        auto ticketId = utils::toInt(parts[3]);
        auto requestId = utils::toInt(parts[4]);
        auto notificationId = utils::toInt(parts[5]);

        if (!userId || !eventId || !ticketId || !requestId || !notificationId) {
            return std::unexpected("Invalid metadata values.");
        }

        ids = {*userId, *eventId, *ticketId, *requestId, *notificationId};
        return {};
    }

    if (parts[0] == "USER") {
        auto user = EntityFactory::userFromRecord(parts);
        if (!user) return std::unexpected(user.error());
        users.add(std::move(*user));
        return {};
    }

    if (parts[0] == "NOTIFICATION") {
        if (parts.size() != 6) return std::unexpected("Invalid notification record.");

        auto userId = utils::toInt(parts[1]);
        auto id = utils::toInt(parts[2]);
        if (!userId || !id) return std::unexpected("Invalid notification ids.");

        User* user = users.findById(*userId);
        if (user == nullptr) return std::unexpected("Notification references an unknown user.");

        user->addNotification(Notification(*id, parts[5], parts[3], parts[4] == "1"));
        return {};
    }

    if (parts[0] == "EVENT") {
        auto event = EntityFactory::eventFromRecord(parts);
        if (!event) return std::unexpected(event.error());
        events.add(std::move(*event));
        return {};
    }

    if (parts[0] == "REQUEST") {
        auto request = EntityFactory::requestFromRecord(parts);
        if (!request) return std::unexpected(request.error());
        requests.add(std::move(*request));
        return {};
    }

    if (parts[0] == "TICKET") {
        auto ticket = EntityFactory::ticketFromRecord(parts);
        if (!ticket) return std::unexpected(ticket.error());
        tickets.add(std::move(*ticket));
        return {};
    }

    return std::unexpected("Unknown record type: " + parts[0]);
}
