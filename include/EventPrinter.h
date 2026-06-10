#ifndef EVENT_PRINTER_H
#define EVENT_PRINTER_H

#include "Event.h"
#include "Client.h"
#include <vector>
#include <memory>

class EventPrinter {
public:
    static void printPublicEvents(const std::vector<std::unique_ptr<Event>>& events);
    static void printOwnedEvents(const std::vector<std::unique_ptr<Event>>& events, int ownerId);
    static void printParticipants(const std::vector<Client>& clients, const std::vector<int>& ids);
};

#endif
