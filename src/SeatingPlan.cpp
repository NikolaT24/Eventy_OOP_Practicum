#include "SeatingPlan.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include "EventyException.h"
#include "Utils.h"

SeatingPlan::SeatingPlan(SeatingMode mode, int capacity, int rows, int columns,
                         int soldCount, std::vector<Seat> occupiedSeats)
    : mode(mode),
      capacity(capacity),
      rows(rows),
      columns(columns),
      soldCount(soldCount),
      occupiedSeats(std::move(occupiedSeats)) {}

SeatingPlan SeatingPlan::generalAdmission(int capacity) {
    if (capacity <= 0) {
        throw ValidationException("Capacity must be positive.");
    }

    return SeatingPlan(SeatingMode::GeneralAdmission, capacity, 0, 0, 0, {});
}

SeatingPlan SeatingPlan::assignedSeats(int rows, int columns) {
    if (rows <= 0 || columns <= 0 || rows > 26) {
        throw ValidationException("Rows must be between 1 and 26 and columns must be positive.");
    }

    return SeatingPlan(SeatingMode::AssignedSeats, rows * columns, rows, columns, 0, {});
}

SeatingPlan SeatingPlan::restore(SeatingMode mode, int capacity, int rows, int columns,
                                 int soldCount, std::vector<Seat> occupiedSeats) {
    if (capacity < 0 || rows < 0 || columns < 0 || soldCount < 0) {
        throw ValidationException("Stored seating data contains negative values.");
    }

    return SeatingPlan(mode, capacity, rows, columns, soldCount, std::move(occupiedSeats));
}

SeatingMode SeatingPlan::getMode() const {
    return mode;
}

int SeatingPlan::getCapacity() const {
    return capacity;
}

int SeatingPlan::getRows() const {
    return rows;
}

int SeatingPlan::getColumns() const {
    return columns;
}

int SeatingPlan::getSoldCount() const {
    return soldCount;
}

int SeatingPlan::getAvailableCount() const {
    return capacity - soldCount;
}

const std::vector<Seat>& SeatingPlan::getOccupiedSeats() const {
    return occupiedSeats;
}

bool SeatingPlan::contains(const Seat& seat) const {
    return std::find(occupiedSeats.begin(), occupiedSeats.end(), seat) != occupiedSeats.end();
}

bool SeatingPlan::isValid(const Seat& seat) const {
    return mode == SeatingMode::AssignedSeats &&
           seat.first >= 0 && seat.first < rows &&
           seat.second >= 0 && seat.second < columns;
}

bool SeatingPlan::isFree(const Seat& seat) const {
    return isValid(seat) && !contains(seat);
}

bool SeatingPlan::canReserve(int count) const {
    return mode == SeatingMode::GeneralAdmission && count > 0 && count <= getAvailableCount();
}

bool SeatingPlan::canReserve(const std::vector<Seat>& seats) const {
    if (mode != SeatingMode::AssignedSeats || seats.empty()) return false;

    std::vector<Seat> uniqueSeats;

    for (const Seat& seat : seats) {
        if (!isFree(seat)) return false;
        if (std::find(uniqueSeats.begin(), uniqueSeats.end(), seat) != uniqueSeats.end()) return false;
        uniqueSeats.push_back(seat);
    }

    return static_cast<int>(seats.size()) <= getAvailableCount();
}

void SeatingPlan::reserve(int count) {
    if (!canReserve(count)) {
        throw InvalidStateException("The requested number of tickets is not available.");
    }

    soldCount += count;
}

void SeatingPlan::reserve(const std::vector<Seat>& seats) {
    if (!canReserve(seats)) {
        throw InvalidStateException("At least one selected seat is invalid, occupied, or duplicated.");
    }

    occupiedSeats.insert(occupiedSeats.end(), seats.begin(), seats.end());
    soldCount += static_cast<int>(seats.size());
}

void SeatingPlan::print(std::ostream& output) const {
    if (mode == SeatingMode::GeneralAdmission) {
        output << "General admission | capacity: " << capacity
               << " | sold: " << soldCount
               << " | available: " << getAvailableCount() << '\n';
        return;
    }

    output << "    ";
    for (int column = 0; column < columns; ++column) {
        output << std::setw(4) << column + 1;
    }
    output << '\n';

    for (int row = 0; row < rows; ++row) {
        output << static_cast<char>('A' + row) << "   ";

        for (int column = 0; column < columns; ++column) {
            output << (contains({row, column}) ? "[X] " : "[ ] ");
        }

        output << '\n';
    }
}

std::expected<Seat, std::string> SeatingPlan::parseSeat(const std::string& value) {
    if (value.size() < 2 || !std::isalpha(static_cast<unsigned char>(value[0]))) {
        return std::unexpected("Invalid seat: " + value);
    }

    const int row = std::toupper(static_cast<unsigned char>(value[0])) - 'A';
    auto column = utils::toInt(value.substr(1));

    if (!column || *column <= 0) {
        return std::unexpected("Invalid seat: " + value);
    }

    return Seat{row, *column - 1};
}

std::string SeatingPlan::seatToString(const Seat& seat) {
    return std::string(1, static_cast<char>('A' + seat.first)) + std::to_string(seat.second + 1);
}

std::string SeatingPlan::encodeSeats(const std::vector<Seat>& seats) {
    std::vector<std::string> values;
    values.reserve(seats.size());

    for (const Seat& seat : seats) {
        values.push_back(seatToString(seat));
    }

    return utils::join(values, ',');
}

std::vector<Seat> SeatingPlan::decodeSeats(const std::string& value) {
    std::vector<Seat> result;

    if (value.empty()) return result;

    for (const std::string& part : utils::split(value, ',')) {
        auto seat = parseSeat(part);
        if (seat) result.push_back(*seat);
    }

    return result;
}
