#include "StorageService.h"
#include "StringUtils.h"
#include "TicketedEvent.h"
#include "VolunteerEvent.h"
#include <fstream>
#include <filesystem>

StorageService::StorageService(const std::string& filePath) {
    this->filePath = filePath;
}

std::expected<void, std::string> StorageService::load(AppState& state) const {
    std::ifstream input(this->filePath);

    if (!input.is_open()) {
        state.ensureAdmin();
        return {};
    }

    state.clear();

    std::string line;
    int lineNumber = 0;

    while (std::getline(input, line)) {
        lineNumber++;

        if (StringUtils::trim(line).empty()) {
            continue;
        }

        auto result = this->loadLine(state, line);

        if (!result.has_value()) {
            return std::unexpected("Line " + std::to_string(lineNumber) + ": " + result.error());
        }
    }

    state.ensureAdmin();
    return {};
}

std::expected<void, std::string> StorageService::save(const AppState& state) const {
    std::filesystem::create_directories(std::filesystem::path(this->filePath).parent_path());

    std::ofstream output(this->filePath);

    if (!output.is_open()) {
        return std::unexpected("Could not open storage file for writing.");
    }

    output << StringUtils::joinEscaped({
        "META",
        std::to_string(state.peekNextClientId()),
        std::to_string(state.peekNextEventId()),
        std::to_string(state.peekNextTicketId()),
        std::to_string(state.peekNextRequestId()),
        std::to_string(state.peekNextNotificationId())
    }, '|') << '\n';

    for (const Client& client : state.getClients()) {
        output << StringUtils::joinEscaped(client.toRecord(), '|') << '\n';
    }

    for (const std::unique_ptr<Event>& event : state.getEvents()) {
        output << StringUtils::joinEscaped(event->toRecord(), '|') << '\n';
    }

    for (const Ticket& ticket : state.getTickets()) {
        output << StringUtils::joinEscaped(ticket.toRecord(), '|') << '\n';
    }

    for (const Request& request : state.getRequests()) {
        output << StringUtils::joinEscaped(request.toRecord(), '|') << '\n';
    }

    for (const Notification& notification : state.getNotifications()) {
        output << StringUtils::joinEscaped(notification.toRecord(), '|') << '\n';
    }

    return {};
}

std::expected<void, std::string> StorageService::loadLine(AppState& state, const std::string& line) const {
    std::vector<std::string> parts = StringUtils::splitEscaped(line, '|');

    if (parts.empty()) {
        return {};
    }

    if (parts[0] == "META") {
        return this->loadMetadata(state, parts);
    }

    if (parts[0] == "CLIENT") {
        return this->loadClient(state, parts);
    }

    if (parts[0] == "EVENT") {
        return this->loadEvent(state, parts);
    }

    if (parts[0] == "TICKET") {
        return this->loadTicket(state, parts);
    }

    if (parts[0] == "REQUEST") {
        return this->loadRequest(state, parts);
    }

    if (parts[0] == "NOTIFICATION") {
        return this->loadNotification(state, parts);
    }

    return std::unexpected("Unknown record type: " + parts[0]);
}

std::expected<void, std::string> StorageService::loadMetadata(AppState& state, const std::vector<std::string>& parts) const {
    if (parts.size() != 6) {
        return std::unexpected("Invalid metadata record.");
    }

    auto clientId = StringUtils::toInt(parts[1]);
    auto eventId = StringUtils::toInt(parts[2]);
    auto ticketId = StringUtils::toInt(parts[3]);
    auto requestId = StringUtils::toInt(parts[4]);
    auto notificationId = StringUtils::toInt(parts[5]);

    if (!clientId || !eventId || !ticketId || !requestId || !notificationId) {
        return std::unexpected("Invalid metadata ids.");
    }

    state.setNextIds(*clientId, *eventId, *ticketId, *requestId, *notificationId);
    return {};
}

std::expected<void, std::string> StorageService::loadClient(AppState& state, const std::vector<std::string>& parts) const {
    if (parts.size() != 6) {
        return std::unexpected("Invalid client record.");
    }

    auto id = StringUtils::toInt(parts[1]);
    auto balance = StringUtils::toDouble(parts[4]);
    auto role = userRoleFromString(parts[5]);

    if (!id || !balance || !role) {
        return std::unexpected("Invalid client data.");
    }

    state.getClients().push_back(Client(*id, parts[2], parts[3], *balance, *role));
    return {};
}

std::expected<void, std::string> StorageService::loadEvent(AppState& state, const std::vector<std::string>& parts) const {
    if (parts.size() < 2) {
        return std::unexpected("Invalid event record.");
    }

    if (parts[1] == "Ticketed") {
        if (parts.size() != 15) {
            return std::unexpected("Invalid ticketed event record.");
        }

        auto id = StringUtils::toInt(parts[2]);
        auto creatorId = StringUtils::toInt(parts[6]);
        auto status = eventStatusFromString(parts[7]);
        auto price = StringUtils::toDouble(parts[8]);
        auto mode = seatingModeFromString(parts[9]);
        auto capacity = StringUtils::toInt(parts[10]);
        auto rows = StringUtils::toInt(parts[11]);
        auto cols = StringUtils::toInt(parts[12]);
        auto sold = StringUtils::toInt(parts[13]);

        if (!id || !creatorId || !status || !price || !mode || !capacity || !rows || !cols || !sold) {
            return std::unexpected("Invalid ticketed event data.");
        }

        SeatingPlan seating = SeatingPlan::fromStored(*mode, *capacity, *rows, *cols, *sold, SeatingPlan::decodeSeats(parts[14]));
        state.getEvents().push_back(std::make_unique<TicketedEvent>(*id, parts[3], parts[4], parts[5], *creatorId, *status, *price, seating));
        return {};
    }

    if (parts[1] == "Volunteer") {
        if (parts.size() != 11) {
            return std::unexpected("Invalid volunteer event record.");
        }

        auto id = StringUtils::toInt(parts[2]);
        auto creatorId = StringUtils::toInt(parts[6]);
        auto status = eventStatusFromString(parts[7]);
        bool applicationsOpen = parts[9] == "1";
        std::vector<int> participants;

        if (!id || !creatorId || !status) {
            return std::unexpected("Invalid volunteer event data.");
        }

        if (!parts[10].empty()) {
            std::vector<std::string> participantParts = StringUtils::splitEscaped(parts[10], ',');

            for (const std::string& participantPart : participantParts) {
                auto participantId = StringUtils::toInt(participantPart);

                if (participantId) {
                    participants.push_back(*participantId);
                }
            }
        }

        state.getEvents().push_back(std::make_unique<VolunteerEvent>(*id, parts[3], parts[4], parts[5], *creatorId, *status, parts[8], applicationsOpen, participants));
        return {};
    }

    return std::unexpected("Unknown event type: " + parts[1]);
}

std::expected<void, std::string> StorageService::loadTicket(AppState& state, const std::vector<std::string>& parts) const {
    if (parts.size() != 9) {
        return std::unexpected("Invalid ticket record.");
    }

    auto id = StringUtils::toInt(parts[1]);
    auto ownerId = StringUtils::toInt(parts[2]);
    auto eventId = StringUtils::toInt(parts[3]);
    auto count = StringUtils::toInt(parts[5]);
    auto totalPrice = StringUtils::toDouble(parts[6]);
    auto type = participationTypeFromString(parts[8]);

    if (!id || !ownerId || !eventId || !count || !totalPrice || !type) {
        return std::unexpected("Invalid ticket data.");
    }

    state.getTickets().push_back(Ticket(*id, *ownerId, *eventId, parts[4], *count, *totalPrice, SeatingPlan::decodeSeats(parts[7]), *type));
    return {};
}

std::expected<void, std::string> StorageService::loadRequest(AppState& state, const std::vector<std::string>& parts) const {
    if (parts.size() != 8) {
        return std::unexpected("Invalid request record.");
    }

    auto id = StringUtils::toInt(parts[1]);
    auto type = requestTypeFromString(parts[2]);
    auto status = requestStatusFromString(parts[3]);
    auto requesterId = StringUtils::toInt(parts[4]);
    auto eventId = StringUtils::toInt(parts[5]);

    if (!id || !type || !status || !requesterId || !eventId) {
        return std::unexpected("Invalid request data.");
    }

    state.getRequests().push_back(Request(*id, *type, *status, *requesterId, *eventId, parts[6], parts[7]));
    return {};
}

std::expected<void, std::string> StorageService::loadNotification(AppState& state, const std::vector<std::string>& parts) const {
    if (parts.size() != 6) {
        return std::unexpected("Invalid notification record.");
    }

    auto id = StringUtils::toInt(parts[1]);
    auto receiverId = StringUtils::toInt(parts[2]);
    bool read = parts[5] == "1";

    if (!id || !receiverId) {
        return std::unexpected("Invalid notification data.");
    }

    state.getNotifications().push_back(Notification(*id, *receiverId, parts[3], parts[4], read));
    return {};
}
