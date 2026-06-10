#ifndef STORAGE_SERVICE_H
#define STORAGE_SERVICE_H

#include <expected>
#include <string>
#include "AppState.h"

class StorageService {
private:
    std::string filePath;

    std::expected<void, std::string> loadLine(AppState& state, const std::string& line) const;
    std::expected<void, std::string> loadMetadata(AppState& state, const std::vector<std::string>& parts) const;
    std::expected<void, std::string> loadClient(AppState& state, const std::vector<std::string>& parts) const;
    std::expected<void, std::string> loadEvent(AppState& state, const std::vector<std::string>& parts) const;
    std::expected<void, std::string> loadTicket(AppState& state, const std::vector<std::string>& parts) const;
    std::expected<void, std::string> loadRequest(AppState& state, const std::vector<std::string>& parts) const;
    std::expected<void, std::string> loadNotification(AppState& state, const std::vector<std::string>& parts) const;

public:
    explicit StorageService(const std::string& filePath);

    std::expected<void, std::string> load(AppState& state) const;
    std::expected<void, std::string> save(const AppState& state) const;
};

#endif
