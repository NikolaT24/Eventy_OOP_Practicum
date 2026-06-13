#pragma once

#include <memory>
#include <string>
#include <vector>
#include "User.h"

class Event;
class Request;

class Admin final : public User {
private:
    std::vector<int> processedRequestIds;

public:
    static constexpr const char* DefaultUsername = "admin";
    static constexpr const char* DefaultPassword = "admin123";

    Admin(int id,
          std::string username = DefaultUsername,
          std::string password = DefaultPassword,
          std::vector<int> processedRequestIds = {},
          std::vector<Notification> notifications = {});

    UserRole getRole() const override;
    std::unique_ptr<User> clone() const override;

    const std::vector<int>& getProcessedRequestIds() const;

    void approvePublishRequest(Request& request, Event& event);
    void rejectPublishRequest(Request& request, Event& event, const std::string& reason);
};
