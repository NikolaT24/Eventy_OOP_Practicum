#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Enums.h"

class Request {
private:
    int id;
    int requesterId;
    int eventId;
    RequestStatus status;
    std::string rejectionReason;

protected:
    Request(int id, int requesterId, int eventId,
            RequestStatus status = RequestStatus::Pending,
            std::string rejectionReason = "");

    std::vector<std::string> baseRecord(const std::string& type) const;

public:
    virtual ~Request() = default;

    Request(const Request&) = default;
    Request& operator=(const Request&) = default;
    Request(Request&&) noexcept = default;
    Request& operator=(Request&&) noexcept = default;

    int getId() const;
    int getRequesterId() const;
    int getEventId() const;
    RequestStatus getStatus() const;
    const std::string& getRejectionReason() const;

    bool isPending() const;
    void approve();
    void reject(const std::string& reason);

    virtual bool isPublishRequest() const = 0;
    virtual bool isVolunteerApplication() const = 0;
    virtual std::string summary() const = 0;
    virtual std::vector<std::string> toRecord() const = 0;
    virtual std::unique_ptr<Request> clone() const = 0;
};

class PublishEventRequest final : public Request {
public:
    PublishEventRequest(int id, int requesterId, int eventId,
                        RequestStatus status = RequestStatus::Pending,
                        std::string rejectionReason = "");

    bool isPublishRequest() const override;
    bool isVolunteerApplication() const override;
    std::string summary() const override;
    std::vector<std::string> toRecord() const override;
    std::unique_ptr<Request> clone() const override;
};

class VolunteerApplicationRequest final : public Request {
private:
    std::string motivation;

public:
    VolunteerApplicationRequest(int id, int requesterId, int eventId,
                                std::string motivation,
                                RequestStatus status = RequestStatus::Pending,
                                std::string rejectionReason = "");

    const std::string& getMotivation() const;

    bool isPublishRequest() const override;
    bool isVolunteerApplication() const override;
    std::string summary() const override;
    std::vector<std::string> toRecord() const override;
    std::unique_ptr<Request> clone() const override;
};
