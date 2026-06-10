#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include "Enums.h"

class Request {
private:
    int id;
    int eventId;
    int creatorId;
    std::string eventTitle;
    RequestStatus status;
    std::string rejectReason;

public:
    Request();

    Request(
        int id,
        int eventId,
        int creatorId,
        const std::string& eventTitle
    );

    int getId() const;
    int getEventId() const;
    int getCreatorId() const;
    std::string getEventTitle() const;

    RequestStatus getStatus() const;
    std::string getStatusAsText() const;

    std::string getRejectReason() const;

    bool isPending() const;

    void approve();
    void reject(const std::string& reason);

    void print() const;
};

#endif
