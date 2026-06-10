#include "EventPrinter.h"
#include <iostream>

void EventPrinter::printPublicEvents(const std::vector<std::unique_ptr<Event>>& events) {
    bool found = false;

    for (const std::unique_ptr<Event>& event : events) {
        if (event->isPublished()) {
            event->printShort();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No published events." << std::endl;
    }
}

void EventPrinter::printOwnedEvents(const std::vector<std::unique_ptr<Event>>& events, int ownerId) {
    bool found = false;

    for (const std::unique_ptr<Event>& event : events) {
        if (event->getCreatorId() == ownerId) {
            event->printShort();
            found = true;
        }
    }

    if (!found) {
        std::cout << "You have not created events yet." << std::endl;
    }
}

void EventPrinter::printParticipants(const std::vector<Client>& clients, const std::vector<int>& ids) {
    if (ids.empty()) {
        std::cout << "No participants yet." << std::endl;
        return;
    }

    for (int id : ids) {
        bool found = false;

        for (const Client& client : clients) {
            if (client.getId() == id) {
                std::cout << "[" << client.getId() << "] " << client.getUsername() << std::endl;
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "[" << id << "] Unknown user" << std::endl;
        }
    }
}
