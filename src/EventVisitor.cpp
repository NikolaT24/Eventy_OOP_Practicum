#include "EventVisitor.h"
#include "TicketedEvent.h"
#include "Utils.h"
#include "VolunteerEvent.h"

namespace {
    std::vector<std::string> baseRecord(const Event& event, const std::string& type) {
        return {
            "EVENT",
            type,
            std::to_string(event.getId()),
            event.getTitle(),
            event.getDate(),
            event.getAddress(),
            std::to_string(event.getCreatorId()),
            toString(event.getStatus()),
            event.getCancellationReason()
        };
    }
}

EventInfoVisitor::EventInfoVisitor(std::ostream& output)
    : output(output) {}

void EventInfoVisitor::visit(const TicketedEvent& event) {
    event.printSummary(output);
    output << "\nType: Ticketed"
           << "\nTicket price: " << event.getTicketPrice()
           << "\nCapacity: " << event.getSeatingPlan().getCapacity()
           << "\nAvailable: " << event.getSeatingPlan().getAvailableCount()
           << "\nSeating mode: " << toString(event.getSeatingPlan().getMode()) << '\n';

    if (event.isCancelled())
        output << "Cancellation reason: " << event.getCancellationReason() << '\n';
}

void EventInfoVisitor::visit(const VolunteerEvent& event) {
    event.printSummary(output);
    output << "\nType: Volunteer"
           << "\nDescription: " << event.getDescription()
           << "\nApplications: " << (event.areApplicationsOpen() ? "OPEN" : "CLOSED")
           << "\nParticipants: " << event.getParticipantIds().size() << '\n';

    if (event.isCancelled())
        output << "Cancellation reason: " << event.getCancellationReason() << '\n';
}

void EventRecordVisitor::visit(const TicketedEvent& event) {
    record = baseRecord(event, "TICKETED");
    record.push_back(std::to_string(event.getTicketPrice()));
    record.push_back(toString(event.getSeatingPlan().getMode()));
    record.push_back(std::to_string(event.getSeatingPlan().getCapacity()));
    record.push_back(std::to_string(event.getSeatingPlan().getRows()));
    record.push_back(std::to_string(event.getSeatingPlan().getColumns()));
    record.push_back(std::to_string(event.getSeatingPlan().getSoldCount()));
    record.push_back(SeatingPlan::encodeSeats(event.getSeatingPlan().getOccupiedSeats()));
}

void EventRecordVisitor::visit(const VolunteerEvent& event) {
    record = baseRecord(event, "VOLUNTEER");
    record.push_back(event.getDescription());
    record.push_back(event.areApplicationsOpen() ? "1" : "0");

    std::vector<std::string> participants;
    for (int id : event.getParticipantIds())
        participants.push_back(std::to_string(id));
    record.push_back(utils::join(participants, ','));
}

const std::vector<std::string>& EventRecordVisitor::getRecord() const {
    return record;
}

