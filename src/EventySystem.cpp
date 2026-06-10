#include "EventySystem.h"
#include <iostream>
#include <sstream>

EventySystem::EventySystem() {
    this->nextClientId = 1;
    this->nextEventId = 1;
    this->currentClientIndex = -1;
    this->running = true;

    this->initializeAdmin();
}

void EventySystem::initializeAdmin() {
    Client admin(
        this->nextClientId,
        "admin",
        "admin123",
        UserRole::Admin
    );

    this->clients.push_back(admin);
    this->nextClientId++;
}

int EventySystem::findClientIndexByUsername(const std::string& username) const {
    for (int i = 0; i < (int)this->clients.size(); i++) {
        if (this->clients[i].getUsername() == username) {
            return i;
        }
    }

    return -1;
}

int EventySystem::findEventIndexById(int eventId) const {
    for (int i = 0; i < (int)this->events.size(); i++) {
        if (this->events[i].getId() == eventId) {
            return i;
        }
    }

    return -1;
}

Client* EventySystem::getCurrentClient() {
    if (this->currentClientIndex < 0) {
        return nullptr;
    }

    return &this->clients[this->currentClientIndex];
}

void EventySystem::run() {
    std::cout << "Welcome to Eventy!" << std::endl;
    std::cout << "Type help to see available commands." << std::endl;

    while (this->running) {
        std::cout << std::endl;

        if (this->currentClientIndex == -1) {
            std::cout << "guest> ";
        }
        else {
            std::cout << this->clients[this->currentClientIndex].getUsername() << "> ";
        }

        std::string line;
        std::getline(std::cin, line);

        if (!line.empty()) {
            this->handleCommand(line);
        }
    }
}

void EventySystem::handleCommand(const std::string& line) {
    std::stringstream input(line);

    std::string command;
    input >> command;

    if (command == "help") {
        this->printHelp();
    }
    else if (command == "exit") {
        this->running = false;
        std::cout << "Goodbye from Eventy." << std::endl;
    }
    else if (command == "register") {
        this->registerClient(input);
    }
    else if (command == "login") {
        this->login(input);
    }
    else if (command == "logout") {
        this->logout();
    }
    else if (command == "wallet") {
        this->showWallet();
    }
    else if (command == "add-balance") {
        this->addBalance(input);
    }
    else if (command == "create-ticketed-event") {
        this->createTicketedEvent(input);
    }
    else if (command == "create-volunteer-event") {
        this->createVolunteerEvent(input);
    }
    else if (command == "list-upcoming-events") {
        this->listUpcomingEvents();
    }
    else if (command == "list-my-events") {
        this->listMyEvents();
    }
    else if (command == "event-info") {
        this->showEventInfo(input);
    }
    else if (command == "list-requests") {
        this->listRequests();
    }
    else if (command == "approve-request") {
        this->approveRequest(input);
    }
    else if (command == "reject-request") {
        this->rejectRequest(input);
    }
    else {
        std::cout << "Unknown command. Type help." << std::endl;
    }
}

void EventySystem::printHelp() const {
    std::cout << "Available commands:" << std::endl;

    if (this->currentClientIndex == -1) {
        std::cout << "  register <username> <password>" << std::endl;
        std::cout << "  login <username> <password>" << std::endl;
        std::cout << "  list-upcoming-events" << std::endl;
        std::cout << "  event-info <event_id>" << std::endl;
        std::cout << "  exit" << std::endl;
        return;
    }

    const Client& client = this->clients[this->currentClientIndex];

    std::cout << "  logout" << std::endl;
    std::cout << "  list-upcoming-events" << std::endl;
    std::cout << "  event-info <event_id>" << std::endl;

    if (client.isAdmin()) {
        std::cout << "  list-requests" << std::endl;
        std::cout << "  approve-request <event_id>" << std::endl;
        std::cout << "  reject-request <event_id>" << std::endl;
    }
    else {
        std::cout << "  wallet" << std::endl;
        std::cout << "  add-balance <amount>" << std::endl;
        std::cout << "  create-ticketed-event <title> <date> <address> <price> <capacity>" << std::endl;
        std::cout << "  create-volunteer-event <title> <date> <address> <description>" << std::endl;
        std::cout << "  list-my-events" << std::endl;
    }

    std::cout << "  exit" << std::endl;
}

void EventySystem::registerClient(std::stringstream& input) {
    if (this->currentClientIndex != -1) {
        std::cout << "Logout first." << std::endl;
        return;
    }

    std::string username;
    std::string password;

    input >> username >> password;

    if (username.empty() || password.empty()) {
        std::cout << "Usage: register <username> <password>" << std::endl;
        return;
    }
    if (this->findClientIndexByUsername(username) != -1) {
        std::cout << "Username already exists." << std::endl;
        return;
    }

    Client client(this->nextClientId, username, password);
    this->clients.push_back(client);
    this->nextClientId++;

    std::cout << "Registered successfully." << std::endl;
}

void EventySystem::login(std::stringstream& input) {
    if (this->currentClientIndex != -1) {
        std::cout << "You are already logged in." << std::endl;
        return;
    }

    std::string username;
    std::string password;

    input >> username >> password;

    if (username.empty() || password.empty()) {
        std::cout << "Usage: login <username> <password>" << std::endl;
        return;
    }

    int index = this->findClientIndexByUsername(username);

    if (index == -1 || !this->clients[index].checkPassword(password)) {
        std::cout << "Invalid username or password." << std::endl;
        return;
    }

    this->currentClientIndex = index;

    std::cout << "Logged in as " << username << "." << std::endl;
}

void EventySystem::logout() {
    if (this->currentClientIndex == -1) {
        std::cout << "You are not logged in." << std::endl;
        return;
    }

    this->currentClientIndex = -1;
    std::cout << "Logged out." << std::endl;
}

void EventySystem::showWallet() const {
    if (this->currentClientIndex == -1) {
        std::cout << "Login first." << std::endl;
        return;
    }

    const Client& client = this->clients[this->currentClientIndex];

    if (client.isAdmin()) {
        std::cout << "Admin does not have a wallet." << std::endl;
        return;
    }

    std::cout << "Balance: " << client.getBalance() << std::endl;
}

void EventySystem::addBalance(std::stringstream& input) {
    Client* client = this->getCurrentClient();

    if (client == nullptr) {
        std::cout << "Login first." << std::endl;
        return;
    }
    if (client->isAdmin()) {
        std::cout << "Admin cannot add balance." << std::endl;
        return;
    }

    double amount;
    input >> amount;

    if (!input || amount <= 0) {
        std::cout << "Usage: add-balance <positive_amount>" << std::endl;
        return;
    }

    client->addBalance(amount);

    std::cout << "Balance updated." << std::endl;
}

void EventySystem::createTicketedEvent(std::stringstream& input) {
    Client* client = this->getCurrentClient();

    if (client == nullptr) {
        std::cout << "Login first." << std::endl;
        return;
    }
    if (client->isAdmin()) {
        std::cout << "Admin cannot create events." << std::endl;
        return;
    }

    std::string title;
    std::string date;
    std::string address;
    double price;
    int capacity;

    input >> title >> date >> address >> price >> capacity;

    if (!input || title.empty() || date.empty() || address.empty() || price < 0 || capacity <= 0) {
        std::cout << "Usage: create-ticketed-event <title> <date> <address> <price> <capacity>" << std::endl;
        return;
    }

    Event event(
        this->nextEventId,
        title,
        date,
        address,
        client->getId(),
        EventType::Ticketed
    );

    event.setTicketInfo(price, capacity);

    this->events.push_back(event);
    this->nextEventId++;

    std::cout << "Ticketed event created. Waiting for admin approval." << std::endl;
}

void EventySystem::createVolunteerEvent(std::stringstream& input) {
    Client* client = this->getCurrentClient();

    if (client == nullptr) {
        std::cout << "Login first." << std::endl;
        return;
    }

    if (client->isAdmin()) {
        std::cout << "Admin cannot create events." << std::endl;
        return;
    }

    std::string title;
    std::string date;
    std::string address;
    std::string description;

    input >> title >> date >> address;
    std::getline(input, description);

    if (title.empty() || date.empty() || address.empty() || description.empty()) {
        std::cout << "Usage: create-volunteer-event <title> <date> <address> <description>" << std::endl;
        return;
    }

    Event event(
        this->nextEventId,
        title,
        date,
        address,
        client->getId(),
        EventType::Volunteer
    );

    event.setVolunteerDescription(description);

    this->events.push_back(event);
    this->nextEventId++;

    std::cout << "Volunteer event created. Waiting for admin approval." << std::endl;
}

void EventySystem::listUpcomingEvents() const {
    bool found = false;

    for (const Event& event : this->events) {
        if (event.isPublished()) {
            std::cout << "[" << event.getId() << "] "
                      << event.getTitle() << " | "
                      << event.getDate() << " | "
                      << event.getAddress() << " | "
                      << event.getTypeAsText() << std::endl;

            found = true;
        }
    }

    if (!found) {
        std::cout << "No published events." << std::endl;
    }
}

void EventySystem::listMyEvents() const {
    if (this->currentClientIndex == -1) {
        std::cout << "Login first." << std::endl;
        return;
    }

    const Client& client = this->clients[this->currentClientIndex];

    if (client.isAdmin()) {
        std::cout << "Admin does not own events." << std::endl;
        return;
    }

    bool found = false;

    for (const Event& event : this->events) {
        if (event.getCreatorId() == client.getId()) {
            std::cout << "[" << event.getId() << "] "
                      << event.getTitle() << " | "
                      << event.getDate() << " | "
                      << event.getTypeAsText() << " | "
                      << event.getStatusAsText() << std::endl;

            found = true;
        }
    }

    if (!found) {
        std::cout << "You have not created events yet." << std::endl;
    }
}

void EventySystem::showEventInfo(std::stringstream& input) const {
    int eventId;
    input >> eventId;

    if (!input) {
        std::cout << "Usage: event-info <event_id>" << std::endl;
        return;
    }

    int index = this->findEventIndexById(eventId);

    if (index == -1) {
        std::cout << "Event not found." << std::endl;
        return;
    }

    const Event& event = this->events[index];

    std::cout << "Id: " << event.getId() << std::endl;
    std::cout << "Title: " << event.getTitle() << std::endl;
    std::cout << "Date: " << event.getDate() << std::endl;
    std::cout << "Address: " << event.getAddress() << std::endl;
    std::cout << "Type: " << event.getTypeAsText() << std::endl;
    std::cout << "Status: " << event.getStatusAsText() << std::endl;

    if (event.isTicketed()) {
        std::cout << "Ticket price: " << event.getTicketPrice() << std::endl;
        std::cout << "Capacity: " << event.getCapacity() << std::endl;
        std::cout << "Available tickets: " << event.getAvailableTickets() << std::endl;
    }
    else {
        std::cout << "Volunteer description: " << event.getVolunteerDescription() << std::endl;
    }
}

void EventySystem::listRequests() const {
    if (this->currentClientIndex == -1 || !this->clients[this->currentClientIndex].isAdmin()) {
        std::cout << "Only admin can view requests." << std::endl;
        return;
    }

    bool found = false;

    for (const Event& event : this->events) {
        if (event.getStatus() == EventStatus::Pending) {
            std::cout << "Request for event [" << event.getId() << "] "
                      << event.getTitle() << " by client id "
                      << event.getCreatorId() << std::endl;

            found = true;
        }
    }

    if (!found) {
        std::cout << "No pending requests." << std::endl;
    }
}

void EventySystem::approveRequest(std::stringstream& input) {
    if (this->currentClientIndex == -1 || !this->clients[this->currentClientIndex].isAdmin()) {
        std::cout << "Only admin can approve requests." << std::endl;
        return;
    }

    int eventId;
    input >> eventId;

    if (!input) {
        std::cout << "Usage: approve-request <event_id>" << std::endl;
        return;
    }

    int index = this->findEventIndexById(eventId);

    if (index == -1) {
        std::cout << "Event request not found." << std::endl;
        return;
    }
    if (this->events[index].getStatus() != EventStatus::Pending) {
        std::cout << "This request is not pending." << std::endl;
        return;
    }

    this->events[index].publish();

    std::cout << "Event approved and published." << std::endl;
}

void EventySystem::rejectRequest(std::stringstream& input) {
    if (this->currentClientIndex == -1 || !this->clients[this->currentClientIndex].isAdmin()) {
        std::cout << "Only admin can reject requests." << std::endl;
        return;
    }

    int eventId;
    input >> eventId;

    if (!input) {
        std::cout << "Usage: reject-request <event_id>" << std::endl;
        return;
    }

    int index = this->findEventIndexById(eventId);

    if (index == -1) {
        std::cout << "Event request not found." << std::endl;
        return;
    }
    if (this->events[index].getStatus() != EventStatus::Pending) {
        std::cout << "This request is not pending." << std::endl;
        return;
    }

    this->events[index].cancel();
    std::cout << "Event request rejected." << std::endl;
}
