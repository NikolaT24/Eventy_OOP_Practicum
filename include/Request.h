#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>
#include "Enums.h"

class Request {
private:
    int id;
    RequestType type;
    RequestStatus status;
    int requesterId;
    int eventId;
    std::string text;
    std::string reason;

public:
    Request();
    Request(int id, RequestType type, int requesterId, int eventId, const std::string& text);
    Request(int id, RequestType type, RequestStatus status, int requesterId, int eventId, const std::string& text, const std::string& reason);

    int getId() const;
    RequestType getType() const;
    RequestStatus getStatus() const;
    int getRequesterId() const;
    int getEventId() const;
    const std::string& getText() const;
    const std::string& getReason() const;

    bool isPending() const;
    bool isPublishRequest() const;
    bool isVolunteerApplication() const;

    void approve();
    void reject(const std::string& reason);
    void print() const;
    std::vector<std::string> toRecord() const;
};

#endif
