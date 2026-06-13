#include "Admin.h"
#include <algorithm>
#include "Event.h"
#include "EventyException.h"
#include "Request.h"

Admin::Admin(int id, std::string username, std::string password, std::vector<int> processedRequestIds, std::vector<Notification> notifications)
    : User(id, std::move(username), std::move(password), std::move(notifications)), processedRequestIds(std::move(processedRequestIds)) {}

UserRole Admin::getRole() const {
    return UserRole::Admin;
}

std::unique_ptr<User> Admin::clone() const {
    return std::make_unique<Admin>(*this);
}

const std::vector<int>& Admin::getProcessedRequestIds() const {
    return processedRequestIds;
}

void Admin::approvePublishRequest(Request& request, Event& event) {
    if (!request.isPublishRequest())
        throw AuthorizationException("Admin can process only event publication requests here.");

    if (!request.isPending())
        throw InvalidStateException("The request has already been processed.");

    event.publish();
    request.approve();

    if (std::find(processedRequestIds.begin(), processedRequestIds.end(), request.getId()) == processedRequestIds.end())
        processedRequestIds.push_back(request.getId());
}

void Admin::rejectPublishRequest(Request& request, Event& event, const std::string& reason) {
    if (!request.isPublishRequest())
        throw AuthorizationException("Admin can process only event publication requests here.");

    if (!request.isPending())
        throw InvalidStateException("The request has already been processed.");

    event.cancel(reason);
    request.reject(reason);

    if (std::find(processedRequestIds.begin(), processedRequestIds.end(), request.getId()) == processedRequestIds.end())
        processedRequestIds.push_back(request.getId());
}
