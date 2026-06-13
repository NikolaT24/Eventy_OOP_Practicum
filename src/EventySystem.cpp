#include "EventySystem.h"
#include <algorithm>
#include <iostream>
#include <set>
#include "Admin.h"
#include "Client.h"
#include "EventVisitor.h"
#include "EventyException.h"
#include "Request.h"
#include "SeatingPlan.h"
#include "TicketedEvent.h"
#include "Utils.h"
#include "VolunteerEvent.h"

namespace {
    void requireExact(const std::vector<std::string>& arguments, std::size_t count,
                      const std::string& usage) {
        if (arguments.size() != count)
            throw ValidationException("Usage: " + usage);
    }

    void requireAtLeast(const std::vector<std::string>& arguments, std::size_t count,
                        const std::string& usage) {
        if (arguments.size() < count)
            throw ValidationException("Usage: " + usage);
    }

    int positiveInt(const std::string& value, const std::string& name) {
        auto parsed = utils::toInt(value);
        if (!parsed || *parsed <= 0)
            throw ValidationException(name + " must be a positive integer.");
        return *parsed;
    }

    double nonNegativeDouble(const std::string& value, const std::string& name) {
        auto parsed = utils::toDouble(value);
        if (!parsed || *parsed < 0)
            throw ValidationException(name + " must be a non-negative number.");
        return *parsed;
    }
}

EventySystem::EventySystem()
    : storage("data/eventy.db") {
    auto loadResult = storage.load(users, events, requests, tickets, ids);

    if (!loadResult) {
        std::cout << "Storage warning: " << loadResult.error() << '\n';
        std::cout << "Starting with a clean state.\n";
        users.clear();
        events.clear();
        requests.clear();
        tickets.clear();
        ids = {};
    }

    ensureAdmin();
    archivePastTickets();
    configureCommands();
}

EventySystem& EventySystem::instance() {
    static EventySystem application;
    return application;
}

void EventySystem::ensureAdmin() {
    if (users.findByUsername(Admin::DefaultUsername) != nullptr) 
        return;
    users.add(std::make_unique<Admin>(ids.nextUserId++));
}

void EventySystem::configureCommands() {
    guestCommands.add("help", [this](const auto& args) { printGuestHelp(args); });
    guestCommands.add("register", [this](const auto& args) { registerClient(args); });
    guestCommands.add("login", [this](const auto& args) { login(args); });
    guestCommands.add("list-upcoming-events", [this](const auto& args) { listUpcomingEvents(args); });
    guestCommands.add("event-info", [this](const auto& args) { showEventInfo(args); });
    guestCommands.add("show-seating", [this](const auto& args) { showSeating(args); });
    guestCommands.add("exit", [this](const auto& args) { exitApplication(args); });

    clientCommands.add("help", [this](const auto& args) { printClientHelp(args); });
    clientCommands.add("logout", [this](const auto& args) { logout(args); });
    clientCommands.add("wallet", [this](const auto& args) { showWallet(args); });
    clientCommands.add("add-balance", [this](const auto& args) { addBalance(args); });
    clientCommands.add("create-event", [this](const auto& args) { createEvent(args); });
    clientCommands.add("create-ticketed-event", [this](const auto& args) { createTicketedAlias(args); });
    clientCommands.add("create-seated-event", [this](const auto& args) { createSeatedAlias(args); });
    clientCommands.add("create-volunteer-event", [this](const auto& args) { createVolunteerAlias(args); });
    clientCommands.add("list-upcoming-events", [this](const auto& args) { listUpcomingEvents(args); });
    clientCommands.add("list-my-events", [this](const auto& args) { listMyEvents(args); });
    clientCommands.add("event-info", [this](const auto& args) { showEventInfo(args); });
    clientCommands.add("show-seating", [this](const auto& args) { showSeating(args); });
    clientCommands.add("buy-ticket", [this](const auto& args) { buyTicket(args); });
    clientCommands.add("list-tickets", [this](const auto& args) { listTickets(args); });
    clientCommands.add("list-history", [this](const auto& args) { listHistory(args); });
    clientCommands.add("volunteer-application", [this](const auto& args) { submitVolunteerApplication(args); });
    clientCommands.add("list-notifications", [this](const auto& args) { listNotifications(args); });
    clientCommands.add("enter-event", [this](const auto& args) { enterEvent(args); });
    clientCommands.add("exit", [this](const auto& args) { exitApplication(args); });

    adminCommands.add("help", [this](const auto& args) { printAdminHelp(args); });
    adminCommands.add("logout", [this](const auto& args) { logout(args); });
    adminCommands.add("list-upcoming-events", [this](const auto& args) { listUpcomingEvents(args); });
    adminCommands.add("event-info", [this](const auto& args) { showEventInfo(args); });
    adminCommands.add("show-seating", [this](const auto& args) { showSeating(args); });
    adminCommands.add("list-requests", [this](const auto& args) { listPublishRequests(args); });
    adminCommands.add("approve-request", [this](const auto& args) { approvePublishRequest(args); });
    adminCommands.add("reject-request", [this](const auto& args) { rejectPublishRequest(args); });
    adminCommands.add("list-notifications", [this](const auto& args) { listNotifications(args); });
    adminCommands.add("exit", [this](const auto& args) { exitApplication(args); });

    managementCommands.add("help", [this](const auto& args) { printManagementHelp(args); });
    managementCommands.add("event-info", [this](const auto& args) { showCurrentEventInfo(args); });
    managementCommands.add("cancel-event", [this](const auto& args) { cancelCurrentEvent(args); });
    managementCommands.add("list-volunteer-applications", [this](const auto& args) { listVolunteerApplications(args); });
    managementCommands.add("approve-application", [this](const auto& args) { approveVolunteerApplication(args); });
    managementCommands.add("reject-application", [this](const auto& args) { rejectVolunteerApplication(args); });
    managementCommands.add("close-volunteer-applications", [this](const auto& args) { closeVolunteerApplications(args); });
    managementCommands.add("list-participants", [this](const auto& args) { listParticipants(args); });
    managementCommands.add("exit-event", [this](const auto& args) { exitEvent(args); });
    managementCommands.add("exit", [this](const auto& args) { exitApplication(args); });
}

User* EventySystem::currentUser() {
    return currentUserId == -1 ? nullptr : users.findById(currentUserId);
}

const User* EventySystem::currentUser() const {
    return currentUserId == -1 ? nullptr : users.findById(currentUserId);
}

Client* EventySystem::currentClient() {
    return dynamic_cast<Client*>(currentUser());
}

const Client* EventySystem::currentClient() const {
    return dynamic_cast<const Client*>(currentUser());
}

Admin* EventySystem::currentAdmin() {
    return dynamic_cast<Admin*>(currentUser());
}

const Admin* EventySystem::currentAdmin() const {
    return dynamic_cast<const Admin*>(currentUser());
}

Event* EventySystem::managedEvent() {
    return managedEventId == -1 ? nullptr : events.findById(managedEventId);
}

const Event* EventySystem::managedEvent() const {
    return managedEventId == -1 ? nullptr : events.findById(managedEventId);
}

void EventySystem::notify(int userId, const std::string& message) {
    User* user = users.findById(userId);
    if (user == nullptr) return;

    user->addNotification(Notification(ids.nextNotificationId++, message, utils::nowText()));
}

void EventySystem::archivePastTickets() {
    std::vector<int> pastEventIds;

    for (const auto& event : events.all()) {
        if (event->getDate() < utils::todayText()) {
            pastEventIds.push_back(event->getId());
        }
    }

    for (int eventId : pastEventIds) {
        std::vector<Ticket> archived = tickets.takeByEvent(eventId);

        for (const Ticket& ticket : archived) {
            if (Client* owner = dynamic_cast<Client*>(users.findById(ticket.getOwnerId()))) {
                owner->removeTicket(ticket.getId());
                owner->addHistoryEvent(eventId);
            }
        }
    }
}

void EventySystem::run() {
    std::cout << "Welcome to Eventy!\nType help to see available commands.\n";

    while (running) {
        if (managedEventId != -1)
            std::cout << "event#" << managedEventId << "> ";
        else if (currentUser() == nullptr)
            std::cout << "guest> ";
        else
            std::cout << currentUser()->getUsername() << "> ";

        std::string line;
        if (!std::getline(std::cin, line)) {
            exitApplication({});
            break;
        }

        ParsedCommand command = parseCommand(line);
        if (command.name.empty()) continue;

        try {
            executeCommand(command);
        } catch (const EventyException& error) {
            std::cout << "Error: " << error.what() << '\n';
        } catch (const std::exception& error) {
            std::cout << "Unexpected error: " << error.what() << '\n';
        }
    }
}

void EventySystem::executeCommand(const ParsedCommand& command) {
    bool executed = false;

    if (managedEventId != -1)
        executed = managementCommands.execute(command);
    else if (currentUser() == nullptr)
        executed = guestCommands.execute(command);
    else if (currentAdmin() != nullptr)
        executed = adminCommands.execute(command);
    else
        executed = clientCommands.execute(command);

    if (!executed)
        std::cout << "Unknown command for the current mode. Type help.\n";
}

void EventySystem::printGuestHelp(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "help");
    std::cout << "register <username> <password>\n"
              << "login <username> <password>\n"
              << "list-upcoming-events\n"
              << "event-info <event_id>\n"
              << "show-seating <event_id>\n"
              << "exit\n";
}

void EventySystem::printClientHelp(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "help");
    std::cout << "logout\nwallet\nadd-balance <amount>\n"
              << "list-upcoming-events\nevent-info <event_id>\nshow-seating <event_id>\n"
              << "buy-ticket <event_id> <count> [A1 A2 ...]\n"
              << "volunteer-application <event_id> <motivation>\n"
              << "list-tickets\nlist-history\n"
              << "create-event ticketed <title> <date> <address> <price> <capacity>\n"
              << "create-event seated <title> <date> <address> <price> <rows> <columns>\n"
              << "create-event volunteer <title> <date> <address> <description>\n"
              << "list-my-events\nenter-event <event_id>\nlist-notifications\nexit\n";
}

void EventySystem::printAdminHelp(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "help");
    std::cout << "list-requests\napprove-request <request_id>\n"
              << "reject-request <request_id> <reason>\n"
              << "list-upcoming-events\nevent-info <event_id>\nshow-seating <event_id>\n"
              << "list-notifications\nlogout\nexit\n";
}

void EventySystem::printManagementHelp(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "help");
    std::cout << "event-info\ncancel-event <reason>\n"
              << "list-volunteer-applications\napprove-application <request_id>\n"
              << "reject-application <request_id> [reason]\n"
              << "close-volunteer-applications\nlist-participants\nexit-event\nexit\n";
}

void EventySystem::registerClient(const std::vector<std::string>& arguments) {
    requireExact(arguments, 2, "register <username> <password>");

    if (users.findByUsername(arguments[0]) != nullptr)
        throw ValidationException("Username is already taken.");
    if (arguments[0].size() < 3 || arguments[1].size() < 4)
        throw ValidationException("Username must have at least 3 characters and password at least 4.");

    users.add(std::make_unique<Client>(ids.nextUserId++, arguments[0], arguments[1]));
    std::cout << "Registration completed successfully.\n";
}

void EventySystem::login(const std::vector<std::string>& arguments) {
    requireExact(arguments, 2, "login <username> <password>");

    User* user = users.findByUsername(arguments[0]);
    if (user == nullptr || !user->checkPassword(arguments[1]))
        throw AuthenticationException("Invalid username or password.");

    currentUserId = user->getId();
    std::cout << "Logged in as " << user->getUsername() << ".\n";
}

void EventySystem::logout(const std::vector<std::string>& arguments) {
    requireExact(arguments, 0, "logout");
    currentUserId = -1;
    managedEventId = -1;
    std::cout << "Logged out.\n";
}

void EventySystem::exitApplication(const std::vector<std::string>& arguments) {
    requireExact(arguments, 0, "exit");

    auto result = storage.save(users, events, requests, tickets, ids);
    if (!result)
        throw EventyException("Could not save data: " + result.error());

    running = false;
    std::cout << "Data saved. Goodbye.\n";
}

void EventySystem::showWallet(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "wallet");
    const Client* client = currentClient();
    if (client == nullptr) 
        throw AuthorizationException("Only clients have a wallet.");
    std::cout << "Balance: " << client->getBalance() << '\n';
}

void EventySystem::addBalance(const std::vector<std::string>& arguments) {
    requireExact(arguments, 1, "add-balance <amount>");
    Client* client = currentClient();
    if (client == nullptr) 
        throw AuthorizationException("Only clients can add balance.");

    auto amount = utils::toDouble(arguments[0]);
    if (!amount) 
        throw ValidationException(amount.error());

    client->addBalance(*amount);
    std::cout << "Balance updated. New balance: " << client->getBalance() << '\n';
}

void EventySystem::createEvent(const std::vector<std::string>& arguments) {
    requireAtLeast(arguments, 1, "create-event <type> ...");

    Client* client = currentClient();
    if (client == nullptr) 
        throw AuthorizationException("Only clients can create events.");

    const std::string& type = arguments[0];
    std::unique_ptr<Event> event;

    if (type == "ticketed") {
        requireExact(arguments, 6, "create-event ticketed <title> <date> <address> <price> <capacity>");
        double price = nonNegativeDouble(arguments[4], "Price");
        int capacity = positiveInt(arguments[5], "Capacity");
        if (!utils::isUpcomingDate(arguments[2])) 
            throw ValidationException("Event date must be valid and not in the past.");

        event = std::make_unique<TicketedEvent>(
            ids.nextEventId++, arguments[1], arguments[2], arguments[3],
            client->getId(), price, SeatingPlan::generalAdmission(capacity)
        );
    } else if (type == "seated") {
        requireExact(arguments, 7, "create-event seated <title> <date> <address> <price> <rows> <columns>");
        double price = nonNegativeDouble(arguments[4], "Price");
        int rows = positiveInt(arguments[5], "Rows");
        int columns = positiveInt(arguments[6], "Columns");
        if (!utils::isUpcomingDate(arguments[2])) throw ValidationException("Event date must be valid and not in the past.");

        event = std::make_unique<TicketedEvent>(
            ids.nextEventId++, arguments[1], arguments[2], arguments[3],
            client->getId(), price, SeatingPlan::assignedSeats(rows, columns)
        );
    } else if (type == "volunteer") {
        requireAtLeast(arguments, 5, "create-event volunteer <title> <date> <address> <description>");
        if (!utils::isUpcomingDate(arguments[2])) 
            throw ValidationException("Event date must be valid and not in the past.");

        event = std::make_unique<VolunteerEvent>(
            ids.nextEventId++, arguments[1], arguments[2], arguments[3],
            client->getId(), utils::joinFrom(arguments, 4)
        );
    } 
    else
        throw ValidationException("Unknown event type. Use ticketed, seated, or volunteer.");

    const int eventId = event->getId();
    events.add(std::move(event));
    requests.add(std::make_unique<PublishEventRequest>(ids.nextRequestId++, client->getId(), eventId));
    client->addCreatedEvent(eventId);

    std::cout << "Event created. Publication request sent to the administrator.\n";
}

void EventySystem::createTicketedAlias(const std::vector<std::string>& arguments) {
    std::vector<std::string> forwarded{"ticketed"};
    forwarded.insert(forwarded.end(), arguments.begin(), arguments.end());
    createEvent(forwarded);
}

void EventySystem::createSeatedAlias(const std::vector<std::string>& arguments) {
    std::vector<std::string> forwarded{"seated"};
    forwarded.insert(forwarded.end(), arguments.begin(), arguments.end());
    createEvent(forwarded);
}

void EventySystem::createVolunteerAlias(const std::vector<std::string>& arguments) {
    std::vector<std::string> forwarded{"volunteer"};
    forwarded.insert(forwarded.end(), arguments.begin(), arguments.end());
    createEvent(forwarded);
}

void EventySystem::listUpcomingEvents(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "list-upcoming-events");
    auto result = events.publishedUpcoming();

    if (result.empty()) {
        std::cout << "No published upcoming events.\n";
        return;
    }

    for (const Event* event : result) {
        event->printSummary(std::cout);
        std::cout << '\n';
    }
}

void EventySystem::listMyEvents(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "list-my-events");
    const Client* client = currentClient();
    if (client == nullptr) 
        throw AuthorizationException("Only clients own events.");

    auto result = events.ownedBy(client->getId());
    if (result.empty()) {
        std::cout << "You have not created events.\n";
        return;
    }

    for (const Event* event : result) {
        event->printSummary(std::cout);
        std::cout << '\n';
    }
}

void EventySystem::showEventInfo(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 1, "event-info <event_id>");
    int eventId = positiveInt(arguments[0], "Event id");

    const Event* event = events.findById(eventId);
    if (event == nullptr) 
        throw NotFoundException("Event not found.");

    const User* user = currentUser();
    bool privileged = user != nullptr &&
        (dynamic_cast<const Admin*>(user) != nullptr || user->getId() == event->getCreatorId());

    if (!event->isPublished() && !privileged)
        throw AuthorizationException("The event is not public.");

    EventInfoVisitor visitor(std::cout);
    event->accept(visitor);
}

void EventySystem::showCurrentEventInfo(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "event-info");
    const Event* event = managedEvent();
    if (event == nullptr) 
        throw NotFoundException("Managed event not found.");

    EventInfoVisitor visitor(std::cout);
    event->accept(visitor);
}

void EventySystem::showSeating(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 1, "show-seating <event_id>");
    int eventId = positiveInt(arguments[0], "Event id");

    const Event* base = events.findById(eventId);
    if (base == nullptr) 
        throw NotFoundException("Event not found.");

    const User* user = currentUser();
    bool privileged = user != nullptr &&
        (dynamic_cast<const Admin*>(user) != nullptr || user->getId() == base->getCreatorId());

    if (!base->isPublished() && !privileged)
        throw AuthorizationException("The event is not public.");

    const auto* event = dynamic_cast<const TicketedEvent*>(base);
    if (event == nullptr) 
        throw ValidationException("Volunteer events do not have seating.");

    event->getSeatingPlan().print(std::cout);
}

void EventySystem::buyTicket(const std::vector<std::string>& arguments) {
    requireAtLeast(arguments, 2, "buy-ticket <event_id> <count> [A1 A2 ...]");
    Client* client = currentClient();
    if (client == nullptr) 
        throw AuthorizationException("Only clients can buy tickets.");

    int eventId = positiveInt(arguments[0], "Event id");
    int count = positiveInt(arguments[1], "Ticket count");

    auto* event = dynamic_cast<TicketedEvent*>(events.findById(eventId));
    if (event == nullptr) 
        throw ValidationException("The event does not sell tickets.");
    if (!event->isPublished() || !utils::isUpcomingDate(event->getDate()))
        throw InvalidStateException("Tickets can be purchased only for an upcoming published event.");

    const double totalPrice = event->priceFor(count);
    if (client->getBalance() < totalPrice)
        throw InvalidStateException("Insufficient balance.");

    std::vector<Seat> selectedSeats;
    if (event->getSeatingPlan().getMode() == SeatingMode::AssignedSeats) {
        if (arguments.size() != static_cast<std::size_t>(count + 2))
            throw ValidationException("Provide exactly one seat for every ticket.");

        for (std::size_t index = 2; index < arguments.size(); ++index) {
            auto seat = SeatingPlan::parseSeat(arguments[index]);
            if (!seat) 
                throw ValidationException(seat.error());
            selectedSeats.push_back(*seat);
        }

        if (!event->getSeatingPlan().canReserve(selectedSeats))
            throw InvalidStateException("At least one selected seat is unavailable.");
    } 
    else {
        requireExact(arguments, 2, "buy-ticket <event_id> <count>");
        if (!event->getSeatingPlan().canReserve(count))
            throw InvalidStateException("Not enough available tickets.");

    }

    client->charge(totalPrice);

    if (selectedSeats.empty())
        event->reserveGeneral(count);
    else
        event->reserveSeats(selectedSeats);

    Ticket ticket(ids.nextTicketId++, client->getId(), eventId, event->getTitle(),
                  count, totalPrice, selectedSeats);
    const int ticketId = ticket.getId();
    tickets.add(std::move(ticket));
    client->addTicket(ticketId);
    client->addHistoryEvent(eventId);

    notify(client->getId(), "Ticket purchase completed for event: " + event->getTitle());
    std::cout << "Tickets purchased successfully. Total: " << totalPrice << '\n';
}

void EventySystem::listTickets(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "list-tickets");
    const Client* client = currentClient();
    if (client == nullptr) 
        throw AuthorizationException("Only clients own tickets.");

    auto result = tickets.byOwner(client->getId());
    if (result.empty()) {
        std::cout << "You do not have active tickets.\n";
        return;
    }

    for (const Ticket* ticket : result)
        std::cout << *ticket << '\n';
}

void EventySystem::listHistory(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "list-history");
    const Client* client = currentClient();
    if (client == nullptr) 
        throw AuthorizationException("Only clients have participation history.");

    bool foundPastParticipation = false;

    for (int eventId : client->getHistoryEventIds()) {
        const Event* event = events.findById(eventId);
        if (event != nullptr && event->getDate() < utils::todayText()) {
            event->printSummary(std::cout);
            std::cout << '\n';
            foundPastParticipation = true;
        }
    }

    if (!foundPastParticipation)
        std::cout << "No past participation history.\n";
}

void EventySystem::submitVolunteerApplication(const std::vector<std::string>& arguments) {
    requireAtLeast(arguments, 2, "volunteer-application <event_id> <motivation>");
    Client* client = currentClient();
    if (client == nullptr) 
        throw AuthorizationException("Only clients can apply as volunteers.");

    int eventId = positiveInt(arguments[0], "Event id");
    auto* event = dynamic_cast<VolunteerEvent*>(events.findById(eventId));

    if (event == nullptr) 
        throw ValidationException("The event is not a volunteer event.");
    if (!event->isPublished() || !utils::isUpcomingDate(event->getDate()))
        throw InvalidStateException("Applications are accepted only for upcoming published events.");
    if (!event->areApplicationsOpen())
        throw InvalidStateException("Volunteer applications are closed.");
    if (event->hasParticipant(client->getId()))
        throw InvalidStateException("You are already a participant.");
    if (requests.hasPendingVolunteerApplication(client->getId(), eventId))
        throw InvalidStateException("You already have a pending application for this event.");

    requests.add(std::make_unique<VolunteerApplicationRequest>(
        ids.nextRequestId++, client->getId(), eventId, utils::joinFrom(arguments, 1)
    ));

    notify(event->getCreatorId(), "New volunteer application for event: " + event->getTitle());
    std::cout << "Volunteer application submitted.\n";
}

void EventySystem::listNotifications(const std::vector<std::string>& arguments) {
    requireExact(arguments, 0, "list-notifications");
    User* user = currentUser();
    if (user == nullptr) 
        throw AuthorizationException("Login is required.");

    if (user->getNotifications().empty()) {
        std::cout << "No notifications.\n";
        return;
    }

    for (Notification& notification : user->getNotifications()) {
        std::cout << notification << '\n';
        notification.markAsRead();
    }
}

void EventySystem::enterEvent(const std::vector<std::string>& arguments) {
    requireExact(arguments, 1, "enter-event <event_id>");
    Client* client = currentClient();
    if (client == nullptr) 
        throw AuthorizationException("Only clients can manage events.");

    int eventId = positiveInt(arguments[0], "Event id");
    Event* event = events.findById(eventId);

    if (event == nullptr) 
        throw NotFoundException("Event not found.");
    
    if (event->getCreatorId() != client->getId())
        throw AuthorizationException("Only the creator can manage this event.");

    managedEventId = eventId;
    std::cout << "Entered event management mode for " << event->getTitle() << ".\n";
}

void EventySystem::exitEvent(const std::vector<std::string>& arguments) {
    requireExact(arguments, 0, "exit-event");
    managedEventId = -1;
    std::cout << "Exited event management mode.\n";
}

void EventySystem::cancelCurrentEvent(const std::vector<std::string>& arguments) {
    requireAtLeast(arguments, 1, "cancel-event <reason>");
    Event* event = managedEvent();
    if (event == nullptr) 
        throw NotFoundException("Managed event not found.");

    std::string reason = utils::joinFrom(arguments, 0);
    event->cancel(reason);

    if (dynamic_cast<TicketedEvent*>(event) != nullptr)
        refundTicketsForCancelledEvent(event->getId(), reason);

    if (auto* volunteer = dynamic_cast<VolunteerEvent*>(event)) {
        for (int participantId : volunteer->getParticipantIds())
            notify(participantId, "Event cancelled: " + event->getTitle() + ". Reason: " + reason);
    }

    for (Request* request : requests.pendingVolunteerApplications(event->getId())) {
        request->reject("Event cancelled: " + reason);
        notify(request->getRequesterId(), "Volunteer application cancelled because the event was cancelled.");
    }

    for (const auto& requestPointer : requests.all()) {
        Request& request = *requestPointer;
        if (request.isPending() && request.isPublishRequest() && request.getEventId() == event->getId())
            request.reject("Event cancelled by its creator: " + reason);
    }

    std::cout << "Event cancelled.\n";
}

void EventySystem::listVolunteerApplications(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "list-volunteer-applications");
    const Event* event = managedEvent();
    if (dynamic_cast<const VolunteerEvent*>(event) == nullptr)
        throw ValidationException("The managed event is not a volunteer event.");

    auto result = requests.pendingVolunteerApplications(event->getId());
    if (result.empty()) {
        std::cout << "No pending volunteer applications.\n";
        return;
    }

    for (const Request* request : result)
        std::cout << request->summary() << '\n';
}

void EventySystem::approveVolunteerApplication(const std::vector<std::string>& arguments) {
    requireExact(arguments, 1, "approve-application <request_id>");
    int requestId = positiveInt(arguments[0], "Request id");

    auto* event = dynamic_cast<VolunteerEvent*>(managedEvent());
    if (event == nullptr) 
        throw ValidationException("The managed event is not a volunteer event.");

    Request* request = requests.findById(requestId);
    if (request == nullptr || !request->isVolunteerApplication() || request->getEventId() != event->getId())
        throw NotFoundException("Volunteer application not found for this event.");
        
    if (!request->isPending()) 
        throw InvalidStateException("The application has already been processed.");

    event->addParticipant(request->getRequesterId());
    request->approve();

    if (Client* client = dynamic_cast<Client*>(users.findById(request->getRequesterId())))
        client->addHistoryEvent(event->getId());

    notify(request->getRequesterId(), "Volunteer application approved for event: " + event->getTitle());
    std::cout << "Volunteer application approved.\n";
}

void EventySystem::rejectVolunteerApplication(const std::vector<std::string>& arguments) {
    requireAtLeast(arguments, 1, "reject-application <request_id> [reason]");
    int requestId = positiveInt(arguments[0], "Request id");

    const auto* event = dynamic_cast<const VolunteerEvent*>(managedEvent());
    if (event == nullptr) 
        throw ValidationException("The managed event is not a volunteer event.");

    Request* request = requests.findById(requestId);
    if (request == nullptr || !request->isVolunteerApplication() || request->getEventId() != event->getId())
        throw NotFoundException("Volunteer application not found for this event.");

    std::string reason = arguments.size() > 1
        ? utils::joinFrom(arguments, 1)
        : "Rejected by the event organizer.";

    request->reject(reason);
    notify(request->getRequesterId(), "Volunteer application rejected for event: " + event->getTitle() + ". Reason: " + reason);
    std::cout << "Volunteer application rejected.\n";
}

void EventySystem::closeVolunteerApplications(const std::vector<std::string>& arguments) {
    requireExact(arguments, 0, "close-volunteer-applications");
    auto* event = dynamic_cast<VolunteerEvent*>(managedEvent());
    if (event == nullptr) 
        throw ValidationException("The managed event is not a volunteer event.");

    event->closeApplications();
    std::cout << "Volunteer applications closed.\n";
}

void EventySystem::listParticipants(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "list-participants");
    const Event* event = managedEvent();
    if (event == nullptr) 
        throw NotFoundException("Managed event not found.");

    std::set<int> participantIds;

    if (const auto* volunteer = dynamic_cast<const VolunteerEvent*>(event))
        participantIds.insert(volunteer->getParticipantIds().begin(), volunteer->getParticipantIds().end());
    else {
        for (const Ticket* ticket : tickets.byEvent(event->getId()))
            participantIds.insert(ticket->getOwnerId());
    }

    if (participantIds.empty()) {
        std::cout << "No participants.\n";
        return;
    }

    for (int id : participantIds) {
        const User* user = users.findById(id);
        if (user != nullptr) 
            std::cout << '[' << id << "] " << user->getUsername() << '\n';
    }
}

void EventySystem::listPublishRequests(const std::vector<std::string>& arguments) const {
    requireExact(arguments, 0, "list-requests");
    if (currentAdmin() == nullptr) 
        throw AuthorizationException("Only the administrator can view publication requests.");

    auto result = requests.pendingPublishRequests();
    if (result.empty()) {
        std::cout << "No pending publication requests.\n";
        return;
    }

    for (const Request* request : result)
        std::cout << request->summary() << '\n';
}

void EventySystem::approvePublishRequest(const std::vector<std::string>& arguments) {
    requireExact(arguments, 1, "approve-request <request_id>");
    Admin* admin = currentAdmin();
    if (admin == nullptr) 
        throw AuthorizationException("Only the administrator can approve publication requests.");

    int requestId = positiveInt(arguments[0], "Request id");
    Request* request = requests.findById(requestId);
    if (request == nullptr || !request->isPublishRequest())
        throw NotFoundException("Publication request not found.");

    Event* event = events.findById(request->getEventId());
    if (event == nullptr) 
        throw NotFoundException("The related event was not found.");

    admin->approvePublishRequest(*request, *event);
    notify(request->getRequesterId(), "Event approved and published: " + event->getTitle());
    std::cout << "Request approved. Event published.\n";
}

void EventySystem::rejectPublishRequest(const std::vector<std::string>& arguments) {
    requireAtLeast(arguments, 2, "reject-request <request_id> <reason>");
    Admin* admin = currentAdmin();
    if (admin == nullptr) 
        throw AuthorizationException("Only the administrator can reject publication requests.");

    int requestId = positiveInt(arguments[0], "Request id");
    Request* request = requests.findById(requestId);
    if (request == nullptr || !request->isPublishRequest())
        throw NotFoundException("Publication request not found.");

    Event* event = events.findById(request->getEventId());
    if (event == nullptr) 
        throw NotFoundException("The related event was not found.");

    std::string reason = utils::joinFrom(arguments, 1);
    admin->rejectPublishRequest(*request, *event, reason);
    notify(request->getRequesterId(), "Event publication rejected: " + event->getTitle() + ". Reason: " + reason);
    std::cout << "Request rejected.\n";
}

void EventySystem::refundTicketsForCancelledEvent(int eventId, const std::string& reason) {
    std::vector<Ticket> refundedTickets = tickets.takeByEvent(eventId);

    for (const Ticket& ticket : refundedTickets) {
        Client* owner = dynamic_cast<Client*>(users.findById(ticket.getOwnerId()));
        if (owner == nullptr) 
            continue;

        owner->refund(ticket.getTotalPrice());
        owner->removeTicket(ticket.getId());
        notify(owner->getId(), "Event cancelled. Refunded " + std::to_string(ticket.getTotalPrice()) +
             ". Reason: " + reason);
    }
}
