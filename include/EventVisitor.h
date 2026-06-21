#pragma once

#include <ostream>
#include <string>
#include <vector>

class TicketedEvent;
class VolunteerEvent;

class EventVisitor {
public:
    virtual ~EventVisitor() = default;
    virtual void visit(const TicketedEvent& event) = 0;
    virtual void visit(const VolunteerEvent& event) = 0;
};

class EventInfoVisitor final : public EventVisitor {
private:
    std::ostream& output;

public:
    explicit EventInfoVisitor(std::ostream& output);

    void visit(const TicketedEvent& event) override;
    void visit(const VolunteerEvent& event) override;
};

class EventRecordVisitor final : public EventVisitor {
private:
    std::vector<std::string> record;

public:
    void visit(const TicketedEvent& event) override;
    void visit(const VolunteerEvent& event) override;

    const std::vector<std::string>& getRecord() const;
};