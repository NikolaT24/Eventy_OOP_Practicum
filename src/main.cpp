#include <iostream>
#include "Client.h"
#include "Event.h"

int main() {
    Client client(1, "nikola", "1234");
    client.addBalance(100);

    Event event(
        1,
        "CppSummit",
        "2026-06-20",
        "Sofia",
        client.getId(),
        EventType::Ticketed
    );

    event.setTicketInfo(25, 100);
    event.publish();

    int wantedTickets = 2;
    double totalPrice = event.getTicketPrice() * wantedTickets;

    std::cout << "Welcome to Eventy!" << std::endl;
    std::cout << "Client: " << client.getUsername() << std::endl;
    std::cout << "Balance: " << client.getBalance() << std::endl;

    std::cout << std::endl;

    std::cout << "Event: " << event.getTitle() << std::endl;
    std::cout << "Type: " << event.getTypeAsText() << std::endl;
    std::cout << "Status: " << event.getStatusAsText() << std::endl;
    std::cout << "Available tickets: " << event.getAvailableTickets() << std::endl;

    std::cout << std::endl;

    if (event.canSellTickets(wantedTickets) && client.withdraw(totalPrice)) {
        event.sellTickets(wantedTickets);

        std::cout << "Bought " << wantedTickets << " tickets successfully." << std::endl;
        std::cout << "Remaining balance: " << client.getBalance() << std::endl;
        std::cout << "Available tickets: " << event.getAvailableTickets() << std::endl;
    }
    else {
        std::cout << "Could not buy tickets." << std::endl;
    }

    return 0;
}
