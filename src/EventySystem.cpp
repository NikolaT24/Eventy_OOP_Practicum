#include "EventySystem.h"
#include "TicketedEvent.h"
#include "VolunteerEvent.h"
#include "EventPrinter.h"
#include "StringUtils.h"
#include "DateUtils.h"
#include <iostream>
#include <memory>

EventySystem::EventySystem() : storage("data/eventy.db") {
    this->currentUserId = -1;
    this->managedEventId = -1;
    this->running = true;

    auto loadResult = this->storage.load(this->state);

    if (!loadResult.has_value()) {
        std::cout << "Storage warning: " << loadResult.error() << std::endl;
        std::cout << "Starting with a clean state." << std::endl;
        this->state.clear();
        this->state.ensureAdmin();
    }
}

Client* EventySystem::currentUser() {
    if (this->currentUserId == -1) {
        return nullptr;
    }

    return this->state.findClientById(this->currentUserId);
}

const Client* EventySystem::currentUser() const {
    if (this->currentUserId == -1) {
        return nullptr;
    }

    return this->state.findClientById(this->currentUserId);
}

Event* EventySystem::managedEvent() {
    if (this->managedEventId == -1) {
        return nullptr;
    }

    return this->state.findEventById(this->managedEventId);
}

const Event* EventySystem::managedEvent() const {
    if (this->managedEventId == -1) {
        return nullptr;
    }

    return this->state.findEventById(this->managedEventId);
}

bool EventySystem::isLoggedIn() const {
    return this->currentUserId != -1;
}

bool EventySystem::isAdmin() const {
    const Client* user = this->currentUser();
    return user != nullptr && user->isAdmin();
}

bool EventySystem::isClient() const {
    const Client* user = this->currentUser();
    return user != nullptr && !user->isAdmin();
}

void EventySystem::run() {
    std::cout << "Welcome to Eventy!" << std::endl;
    std::cout << "Type help to see commands." << std::endl;

    while (this->running) {
        std::cout << std::endl;

        const Client* user = this->currentUser();

        if (this->managedEventId != -1) {
            std::cout << "event#" << this->managedEventId << "> ";
        } else if (user == nullptr) {
            std::cout << "guest> ";
        } else {
            std::cout << user->getUsername() << "> ";
        }

        std::string line;
        std::getline(std::cin, line);

        ParsedCommand command = CommandParser::parse(line);

        if (!command.name.empty()) {
            this->handleCommand(command);
        }
    }
}

void EventySystem::handleCommand(const ParsedCommand& command) {
    if (this->managedEventId != -1) {
        this->handleEventManagementCommand(command);
    } else if (!this->isLoggedIn()) {
        this->handleGuestCommand(command);
    } else if (this->isAdmin()) {
        this->handleAdminCommand(command);
    } else {
        this->handleClientCommand(command);
    }
}

void EventySystem::handleGuestCommand(const ParsedCommand& command) {
    if (command.name == "help") {
        this->printGuestHelp();
    } else if (command.name == "register") {
        this->registerClient(command.args);
    } else if (command.name == "login") {
        this->login(command.args);
    } else if (command.name == "list-upcoming-events") {
        this->listUpcomingEvents();
    } else if (command.name == "event-info") {
        this->showEventInfo(command.args);
    } else if (command.name == "show-seating") {
        this->showSeating(command.args);
    } else if (command.name == "exit") {
        this->exitApplication();
    } else {
        std::cout << "Unknown guest command. Type help." << std::endl;
    }
}

void EventySystem::handleAdminCommand(const ParsedCommand& command) {
    if (command.name == "help") {
        this->printAdminHelp();
    } else if (command.name == "logout") {
        this->logout();
    } else if (command.name == "list-upcoming-events") {
        this->listUpcomingEvents();
    } else if (command.name == "event-info") {
        this->showEventInfo(command.args);
    } else if (command.name == "show-seating") {
        this->showSeating(command.args);
    } else if (command.name == "list-notifications") {
        this->listNotifications();
    } else if (command.name == "list-requests") {
        this->listPublishRequests();
    } else if (command.name == "approve-request") {
        this->approvePublishRequest(command.args);
    } else if (command.name == "reject-request") {
        this->rejectPublishRequest(command.args);
    } else if (command.name == "exit") {
        this->exitApplication();
    } else {
        std::cout << "Unknown admin command. Type help." << std::endl;
    }
}

void EventySystem::handleClientCommand(const ParsedCommand& command) {
    if (command.name == "help") {
        this->printClientHelp();
    } else if (command.name == "logout") {
        this->logout();
    } else if (command.name == "wallet") {
        this->showWallet();
    } else if (command.name == "add-balance") {
        this->addBalance(command.args);
    } else if (command.name == "create-ticketed-event") {
        this->createTicketedEvent(command.args);
    } else if (command.name == "create-seated-event") {
        this->createSeatedEvent(command.args);
    } else if (command.name == "create-volunteer-event") {
        this->createVolunteerEvent(command.args);
    } else if (command.name == "list-upcoming-events") {
        this->listUpcomingEvents();
    } else if (command.name == "list-my-events") {
        this->listMyEvents();
    } else if (command.name == "event-info") {
        this->showEventInfo(command.args);
    } else if (command.name == "show-seating") {
        this->showSeating(command.args);
    } else if (command.name == "buy-ticket") {
        this->buyTicket(command.args);
    } else if (command.name == "list-tickets") {
        this->listTickets();
    } else if (command.name == "list-history") {
        this->listHistory();
    } else if (command.name == "volunteer-application") {
        this->submitVolunteerApplication(command.args);
    } else if (command.name == "list-notifications") {
        this->listNotifications();
    } else if (command.name == "enter-event") {
        this->enterEvent(command.args);
    } else if (command.name == "exit") {
        this->exitApplication();
    } else {
        std::cout << "Unknown client command. Type help." << std::endl;
    }
}

void EventySystem::handleEventManagementCommand(const ParsedCommand& command) {
    if (command.name == "help") {
        this->printEventManagementHelp();
    } else if (command.name == "event-info") {
        this->showCurrentEventInfo();
    } else if (command.name == "cancel-event") {
        this->cancelCurrentEvent(command.args);
    } else if (command.name == "list-volunteer-applications") {
        this->listVolunteerApplications();
    } else if (command.name == "approve-application") {
        this->approveVolunteerApplication(command.args);
    } else if (command.name == "reject-application") {
        this->rejectVolunteerApplication(command.args);
    } else if (command.name == "close-volunteer-applications") {
        this->closeVolunteerApplications();
    } else if (command.name == "list-participants") {
        this->listParticipants();
    } else if (command.name == "show-seating") {
        this->showSeating({ std::to_string(this->managedEventId) });
    } else if (command.name == "exit-event") {
        this->exitEvent();
    } else if (command.name == "exit") {
        this->exitApplication();
    } else {
        std::cout << "Unknown event-management command. Type help." << std::endl;
    }
}

void EventySystem::printGuestHelp() const {
    std::cout << "Guest commands:" << std::endl;
    std::cout << "  register <username> <password>" << std::endl;
    std::cout << "  login <username> <password>" << std::endl;
    std::cout << "  list-upcoming-events" << std::endl;
    std::cout << "  event-info <event_id>" << std::endl;
    std::cout << "  show-seating <event_id>" << std::endl;
    std::cout << "  exit" << std::endl;
}

void EventySystem::printClientHelp() const {
    std::cout << "Client commands:" << std::endl;
    std::cout << "  logout" << std::endl;
    std::cout << "  wallet" << std::endl;
    std::cout << "  add-balance <amount>" << std::endl;
    std::cout << "  create-ticketed-event <title> <date> <address> <price> <capacity>" << std::endl;
    std::cout << "  create-seated-event <title> <date> <address> <price> <rows> <cols>" << std::endl;
    std::cout << "  create-volunteer-event <title> <date> <address> <description>" << std::endl;
    std::cout << "  list-upcoming-events" << std::endl;
    std::cout << "  list-my-events" << std::endl;
    std::cout << "  event-info <event_id>" << std::endl;
    std::cout << "  show-seating <event_id>" << std::endl;
    std::cout << "  buy-ticket <event_id> <count> [row col ...]" << std::endl;
    std::cout << "  volunteer-application <event_id> <motivation>" << std::endl;
    std::cout << "  list-tickets" << std::endl;
    std::cout << "  list-history" << std::endl;
    std::cout << "  list-notifications" << std::endl;
    std::cout << "  enter-event <event_id>" << std::endl;
    std::cout << "  exit" << std::endl;
}

void EventySystem::printAdminHelp() const {
    std::cout << "Admin commands:" << std::endl;
    std::cout << "  list-requests" << std::endl;
    std::cout << "  approve-request <request_id>" << std::endl;
    std::cout << "  reject-request <request_id> <reason>" << std::endl;
    std::cout << "  list-notifications" << std::endl;
    std::cout << "  logout" << std::endl;
    std::cout << "  exit" << std::endl;
}

void EventySystem::printEventManagementHelp() const {
    std::cout << "Event management commands:" << std::endl;
    std::cout << "  event-info" << std::endl;
    std::cout << "  cancel-event <reason>" << std::endl;
    std::cout << "  list-volunteer-applications" << std::endl;
    std::cout << "  approve-application <request_id>" << std::endl;
    std::cout << "  reject-application <request_id> <reason>" << std::endl;
    std::cout << "  close-volunteer-applications" << std::endl;
    std::cout << "  list-participants" << std::endl;
    std::cout << "  show-seating" << std::endl;
    std::cout << "  exit-event" << std::endl;
    std::cout << "  exit" << std::endl;
}

void EventySystem::registerClient(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Usage: register <username> <password>" << std::endl;
        return;
    }

    if (this->state.findClientByUsername(args[0]) != nullptr) {
        std::cout << "Username already exists." << std::endl;
        return;
    }

    this->state.getClients().push_back(Client(this->state.nextClientId(), args[0], args[1]));
    std::cout << "Registration successful." << std::endl;
}

void EventySystem::login(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Usage: login <username> <password>" << std::endl;
        return;
    }

    Client* client = this->state.findClientByUsername(args[0]);

    if (client == nullptr || !client->checkPassword(args[1])) {
        std::cout << "Invalid username or password." << std::endl;
        return;
    }

    this->currentUserId = client->getId();
    std::cout << "Logged in as " << client->getUsername() << "." << std::endl;
}

void EventySystem::logout() {
    this->managedEventId = -1;
    this->currentUserId = -1;
    std::cout << "Logged out." << std::endl;
}

void EventySystem::exitApplication() {
    auto saveResult = this->storage.save(this->state);

    if (!saveResult.has_value()) {
        std::cout << "Save error: " << saveResult.error() << std::endl;
    } else {
        std::cout << "Data saved." << std::endl;
    }

    this->running = false;
}

void EventySystem::showWallet() const {
    const Client* user = this->currentUser();
    std::cout << "Balance: " << user->getBalance() << std::endl;
}

void EventySystem::addBalance(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Usage: add-balance <amount>" << std::endl;
        return;
    }

    auto amount = StringUtils::toDouble(args[0]);

    if (!amount || *amount <= 0) {
        std::cout << "Amount must be positive." << std::endl;
        return;
    }

    this->currentUser()->addBalance(*amount);
    std::cout << "Balance updated." << std::endl;
}

void EventySystem::createTicketedEvent(const std::vector<std::string>& args) {
    if (args.size() != 5) {
        std::cout << "Usage: create-ticketed-event <title> <date> <address> <price> <capacity>" << std::endl;
        return;
    }

    auto price = StringUtils::toDouble(args[3]);
    auto capacity = StringUtils::toInt(args[4]);

    if (!DateUtils::isValidDate(args[1]) || !price || !capacity || *price < 0 || *capacity <= 0) {
        std::cout << "Invalid event data." << std::endl;
        return;
    }

    int eventId = this->state.nextEventId();
    int creatorId = this->currentUser()->getId();

    this->state.getEvents().push_back(std::make_unique<TicketedEvent>(eventId, args[0], args[1], args[2], creatorId, *price, SeatingPlan::general(*capacity)));
    this->state.getRequests().push_back(Request(this->state.nextRequestId(), RequestType::PublishEvent, creatorId, eventId, "Publish event: " + args[0]));

    std::cout << "Ticketed event created. Publication request sent to admin." << std::endl;
}

void EventySystem::createSeatedEvent(const std::vector<std::string>& args) {
    if (args.size() != 6) {
        std::cout << "Usage: create-seated-event <title> <date> <address> <price> <rows> <cols>" << std::endl;
        return;
    }

    auto price = StringUtils::toDouble(args[3]);
    auto rows = StringUtils::toInt(args[4]);
    auto cols = StringUtils::toInt(args[5]);

    if (!DateUtils::isValidDate(args[1]) || !price || !rows || !cols || *price < 0 || *rows <= 0 || *cols <= 0) {
        std::cout << "Invalid seated event data." << std::endl;
        return;
    }

    int eventId = this->state.nextEventId();
    int creatorId = this->currentUser()->getId();

    this->state.getEvents().push_back(std::make_unique<TicketedEvent>(eventId, args[0], args[1], args[2], creatorId, *price, SeatingPlan::assigned(*rows, *cols)));
    this->state.getRequests().push_back(Request(this->state.nextRequestId(), RequestType::PublishEvent, creatorId, eventId, "Publish event: " + args[0]));

    std::cout << "Seated event created. Publication request sent to admin." << std::endl;
}

void EventySystem::createVolunteerEvent(const std::vector<std::string>& args) {
    if (args.size() < 4) {
        std::cout << "Usage: create-volunteer-event <title> <date> <address> <description>" << std::endl;
        return;
    }

    if (!DateUtils::isValidDate(args[1])) {
        std::cout << "Invalid date. Use YYYY-MM-DD." << std::endl;
        return;
    }

    std::string description = StringUtils::joinFrom(args, 3);
    int eventId = this->state.nextEventId();
    int creatorId = this->currentUser()->getId();

    this->state.getEvents().push_back(std::make_unique<VolunteerEvent>(eventId, args[0], args[1], args[2], creatorId, description));
    this->state.getRequests().push_back(Request(this->state.nextRequestId(), RequestType::PublishEvent, creatorId, eventId, "Publish event: " + args[0]));

    std::cout << "Volunteer event created. Publication request sent to admin." << std::endl;
}

void EventySystem::listUpcomingEvents() const {
    EventPrinter::printPublicEvents(this->state.getEvents());
}

void EventySystem::listMyEvents() const {
    EventPrinter::printOwnedEvents(this->state.getEvents(), this->currentUser()->getId());
}

void EventySystem::showEventInfo(const std::vector<std::string>& args) const {
    if (args.size() != 1) {
        std::cout << "Usage: event-info <event_id>" << std::endl;
        return;
    }

    auto eventId = StringUtils::toInt(args[0]);

    if (!eventId) {
        std::cout << "Invalid event id." << std::endl;
        return;
    }

    const Event* event = this->state.findEventById(*eventId);

    if (event == nullptr) {
        std::cout << "Event not found." << std::endl;
        return;
    }

    if (!event->isPublished()) {
        const Client* user = this->currentUser();
        bool allowed = user != nullptr && (user->isAdmin() || user->getId() == event->getCreatorId());

        if (!allowed) {
            std::cout << "Event is not public." << std::endl;
            return;
        }
    }

    event->printInfo();
}

void EventySystem::showCurrentEventInfo() const {
    const Event* event = this->managedEvent();

    if (event != nullptr) {
        event->printInfo();
    }
}

void EventySystem::showSeating(const std::vector<std::string>& args) const {
    if (args.size() != 1) {
        std::cout << "Usage: show-seating <event_id>" << std::endl;
        return;
    }

    auto eventId = StringUtils::toInt(args[0]);

    if (!eventId) {
        std::cout << "Invalid event id." << std::endl;
        return;
    }

    const Event* event = this->state.findEventById(*eventId);
    const TicketedEvent* ticketedEvent = dynamic_cast<const TicketedEvent*>(event);

    if (ticketedEvent == nullptr) {
        std::cout << "This is not a ticketed event." << std::endl;
        return;
    }

    ticketedEvent->getSeating().print();
}

void EventySystem::buyTicket(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: buy-ticket <event_id> <count> [row col ...]" << std::endl;
        return;
    }

    auto eventId = StringUtils::toInt(args[0]);
    auto count = StringUtils::toInt(args[1]);

    if (!eventId || !count || *count <= 0) {
        std::cout << "Invalid ticket command." << std::endl;
        return;
    }

    Event* event = this->state.findEventById(*eventId);
    TicketedEvent* ticketedEvent = dynamic_cast<TicketedEvent*>(event);

    if (ticketedEvent == nullptr) {
        std::cout << "Event does not sell tickets." << std::endl;
        return;
    }

    if (!ticketedEvent->isPublished()) {
        std::cout << "Event is not published." << std::endl;
        return;
    }

    std::vector<Seat> seats;
    bool assigned = ticketedEvent->getSeating().getMode() == SeatingMode::AssignedSeats;

    if (assigned) {
        if ((int)args.size() != 2 + (*count * 2)) {
            std::cout << "Assigned seating requires row and column for every ticket." << std::endl;
            return;
        }

        for (int i = 0; i < *count; i++) {
            auto row = StringUtils::toInt(args[2 + i * 2]);
            auto col = StringUtils::toInt(args[3 + i * 2]);

            if (!row || !col) {
                std::cout << "Invalid seat coordinates." << std::endl;
                return;
            }

            seats.push_back({ *row, *col });
        }

        if (!ticketedEvent->canBuySeats(seats)) {
            std::cout << "Selected seats are not available." << std::endl;
            return;
        }
    } else if (!ticketedEvent->canBuyGeneral(*count)) {
        std::cout << "Not enough available tickets." << std::endl;
        return;
    }

    double totalPrice = ticketedEvent->getPrice() * (*count);
    Client* user = this->currentUser();

    if (!user->withdraw(totalPrice)) {
        std::cout << "Not enough balance." << std::endl;
        return;
    }

    if (assigned) {
        ticketedEvent->buySeats(seats);
    } else {
        ticketedEvent->buyGeneral(*count);
    }

    this->state.getTickets().push_back(Ticket(this->state.nextTicketId(), user->getId(), ticketedEvent->getId(), ticketedEvent->getTitle(), *count, totalPrice, seats, ParticipationType::Ticket));
    this->state.addNotification(user->getId(), "You bought tickets for " + ticketedEvent->getTitle() + ".");

    std::cout << "Tickets bought successfully." << std::endl;
}

void EventySystem::listTickets() const {
    bool found = false;
    int userId = this->currentUser()->getId();

    for (const Ticket& ticket : this->state.getTickets()) {
        if (ticket.getOwnerId() == userId && ticket.getParticipationType() == ParticipationType::Ticket) {
            ticket.print();
            std::cout << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "You do not have tickets." << std::endl;
    }
}

void EventySystem::listHistory() const {
    bool found = false;
    int userId = this->currentUser()->getId();

    for (const Ticket& ticket : this->state.getTickets()) {
        if (ticket.getOwnerId() == userId) {
            ticket.print();
            std::cout << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No participation history." << std::endl;
    }
}

void EventySystem::submitVolunteerApplication(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: volunteer-application <event_id> <motivation>" << std::endl;
        return;
    }

    auto eventId = StringUtils::toInt(args[0]);

    if (!eventId) {
        std::cout << "Invalid event id." << std::endl;
        return;
    }

    Event* event = this->state.findEventById(*eventId);
    VolunteerEvent* volunteerEvent = dynamic_cast<VolunteerEvent*>(event);

    if (volunteerEvent == nullptr || !volunteerEvent->isPublished()) {
        std::cout << "Volunteer event not found or not published." << std::endl;
        return;
    }

    if (!volunteerEvent->areApplicationsOpen()) {
        std::cout << "Applications are closed." << std::endl;
        return;
    }

    int userId = this->currentUser()->getId();

    if (volunteerEvent->hasParticipant(userId) || this->hasActiveVolunteerApplication(userId, *eventId)) {
        std::cout << "You already have an application or participation for this event." << std::endl;
        return;
    }

    std::string motivation = StringUtils::joinFrom(args, 1);
    this->state.getRequests().push_back(Request(this->state.nextRequestId(), RequestType::VolunteerApplication, userId, *eventId, motivation));
    this->state.addNotification(volunteerEvent->getCreatorId(), "New volunteer application for " + volunteerEvent->getTitle() + ".");

    std::cout << "Volunteer application sent." << std::endl;
}

void EventySystem::listNotifications() {
    bool found = false;
    int userId = this->currentUser()->getId();

    for (Notification& notification : this->state.getNotifications()) {
        if (notification.getReceiverId() == userId) {
            notification.print();
            notification.markAsRead();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No notifications." << std::endl;
    }
}

void EventySystem::enterEvent(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Usage: enter-event <event_id>" << std::endl;
        return;
    }

    auto eventId = StringUtils::toInt(args[0]);

    if (!eventId) {
        std::cout << "Invalid event id." << std::endl;
        return;
    }

    Event* event = this->state.findEventById(*eventId);

    if (event == nullptr || event->getCreatorId() != this->currentUser()->getId()) {
        std::cout << "You can manage only your own events." << std::endl;
        return;
    }

    this->managedEventId = *eventId;
    std::cout << "Entered event management mode for " << event->getTitle() << "." << std::endl;
}

void EventySystem::exitEvent() {
    this->managedEventId = -1;
    std::cout << "Returned to client mode." << std::endl;
}

void EventySystem::cancelCurrentEvent(const std::vector<std::string>& args) {
    Event* event = this->managedEvent();

    if (event == nullptr) {
        return;
    }

    std::string reason = args.empty() ? "No reason provided." : StringUtils::joinFrom(args, 0);
    event->cancel();
    this->refundTicketsForCancelledEvent(event->getId(), reason);
    std::cout << "Event cancelled." << std::endl;
}

void EventySystem::listVolunteerApplications() const {
    const Event* event = this->managedEvent();
    bool found = false;

    for (const Request& request : this->state.getRequests()) {
        if (request.isPending() && request.isVolunteerApplication() && request.getEventId() == event->getId()) {
            request.print();
            std::cout << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No pending volunteer applications." << std::endl;
    }
}

void EventySystem::approveVolunteerApplication(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Usage: approve-application <request_id>" << std::endl;
        return;
    }

    auto requestId = StringUtils::toInt(args[0]);

    if (!requestId) {
        std::cout << "Invalid request id." << std::endl;
        return;
    }

    Request* request = this->state.findRequestById(*requestId);
    VolunteerEvent* event = dynamic_cast<VolunteerEvent*>(this->managedEvent());

    if (request == nullptr || event == nullptr || !request->isPending() || !request->isVolunteerApplication() || request->getEventId() != event->getId()) {
        std::cout << "Application not found for this event." << std::endl;
        return;
    }

    event->addParticipant(request->getRequesterId());
    request->approve();

    this->state.getTickets().push_back(Ticket(this->state.nextTicketId(), request->getRequesterId(), event->getId(), event->getTitle(), 1, 0, {}, ParticipationType::Volunteer));
    this->state.addNotification(request->getRequesterId(), "Your volunteer application was approved for " + event->getTitle() + ".");

    std::cout << "Application approved." << std::endl;
}

void EventySystem::rejectVolunteerApplication(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: reject-application <request_id> <reason>" << std::endl;
        return;
    }

    auto requestId = StringUtils::toInt(args[0]);

    if (!requestId) {
        std::cout << "Invalid request id." << std::endl;
        return;
    }

    Request* request = this->state.findRequestById(*requestId);
    Event* event = this->managedEvent();

    if (request == nullptr || event == nullptr || !request->isPending() || !request->isVolunteerApplication() || request->getEventId() != event->getId()) {
        std::cout << "Application not found for this event." << std::endl;
        return;
    }

    std::string reason = StringUtils::joinFrom(args, 1);
    request->reject(reason);
    this->state.addNotification(request->getRequesterId(), "Your volunteer application was rejected. Reason: " + reason);

    std::cout << "Application rejected." << std::endl;
}

void EventySystem::closeVolunteerApplications() {
    VolunteerEvent* event = dynamic_cast<VolunteerEvent*>(this->managedEvent());

    if (event == nullptr) {
        std::cout << "This command is only for volunteer events." << std::endl;
        return;
    }

    if (!event->isPublished() || event->isCancelled()) {
        std::cout << "Applications can be closed only for active published volunteer events." << std::endl;
        return;
    }

    event->closeApplications();
    std::cout << "Volunteer applications closed." << std::endl;
}

void EventySystem::listParticipants() const {
    const Event* event = this->managedEvent();
    const VolunteerEvent* volunteerEvent = dynamic_cast<const VolunteerEvent*>(event);

    if (volunteerEvent != nullptr) {
        EventPrinter::printParticipants(this->state.getClients(), volunteerEvent->getParticipantIds());
        return;
    }

    std::vector<int> participantIds;

    for (const Ticket& ticket : this->state.getTickets()) {
        if (ticket.getEventId() == event->getId()) {
            participantIds.push_back(ticket.getOwnerId());
        }
    }

    EventPrinter::printParticipants(this->state.getClients(), participantIds);
}

void EventySystem::listPublishRequests() const {
    bool found = false;

    for (const Request& request : this->state.getRequests()) {
        if (request.isPending() && request.isPublishRequest()) {
            request.print();
            std::cout << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No pending publish requests." << std::endl;
    }
}

void EventySystem::approvePublishRequest(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Usage: approve-request <request_id>" << std::endl;
        return;
    }

    auto requestId = StringUtils::toInt(args[0]);

    if (!requestId) {
        std::cout << "Invalid request id." << std::endl;
        return;
    }

    Request* request = this->state.findRequestById(*requestId);

    if (request == nullptr || !request->isPending() || !request->isPublishRequest()) {
        std::cout << "Pending publish request not found." << std::endl;
        return;
    }

    Event* event = this->state.findEventById(request->getEventId());

    if (event == nullptr) {
        std::cout << "Connected event not found." << std::endl;
        return;
    }

    event->publish();
    request->approve();
    this->state.addNotification(request->getRequesterId(), "Your event was approved: " + event->getTitle() + ".");

    std::cout << "Request approved. Event published." << std::endl;
}

void EventySystem::rejectPublishRequest(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: reject-request <request_id> <reason>" << std::endl;
        return;
    }

    auto requestId = StringUtils::toInt(args[0]);

    if (!requestId) {
        std::cout << "Invalid request id." << std::endl;
        return;
    }

    Request* request = this->state.findRequestById(*requestId);

    if (request == nullptr || !request->isPending() || !request->isPublishRequest()) {
        std::cout << "Pending publish request not found." << std::endl;
        return;
    }

    Event* event = this->state.findEventById(request->getEventId());

    if (event == nullptr) {
        std::cout << "Connected event not found." << std::endl;
        return;
    }

    std::string reason = StringUtils::joinFrom(args, 1);
    event->cancel();
    request->reject(reason);
    this->state.addNotification(request->getRequesterId(), "Your event was rejected: " + event->getTitle() + ". Reason: " + reason);

    std::cout << "Request rejected." << std::endl;
}

bool EventySystem::hasActiveVolunteerApplication(int userId, int eventId) const {
    for (const Request& request : this->state.getRequests()) {
        if (request.isPending() && request.isVolunteerApplication() && request.getRequesterId() == userId && request.getEventId() == eventId) {
            return true;
        }
    }

    return false;
}

void EventySystem::refundTicketsForCancelledEvent(int eventId, const std::string& reason) {
    for (const Ticket& ticket : this->state.getTickets()) {
        if (ticket.getEventId() == eventId && ticket.getParticipationType() == ParticipationType::Ticket) {
            Client* owner = this->state.findClientById(ticket.getOwnerId());

            if (owner != nullptr) {
                owner->addBalance(ticket.getTotalPrice());
                this->state.addNotification(owner->getId(), "Event cancelled. Refund received for " + ticket.getEventTitle() + ". Reason: " + reason);
            }
        }
    }
}
