#include "SeatingPlan.h"
#include "StringUtils.h"
#include <iostream>
#include <set>

SeatingPlan::SeatingPlan() {
    this->mode = SeatingMode::GeneralAdmission;
    this->capacity = 0;
    this->rows = 0;
    this->cols = 0;
    this->soldCount = 0;
}

SeatingPlan SeatingPlan::general(int capacity) {
    SeatingPlan plan;
    plan.mode = SeatingMode::GeneralAdmission;
    plan.capacity = capacity;
    plan.rows = 0;
    plan.cols = 0;
    plan.soldCount = 0;
    return plan;
}

SeatingPlan SeatingPlan::assigned(int rows, int cols) {
    SeatingPlan plan;
    plan.mode = SeatingMode::AssignedSeats;
    plan.capacity = rows * cols;
    plan.rows = rows;
    plan.cols = cols;
    plan.soldCount = 0;
    return plan;
}

SeatingPlan SeatingPlan::fromStored(SeatingMode mode, int capacity, int rows, int cols, int soldCount, const std::vector<Seat>& occupiedSeats) {
    SeatingPlan plan;
    plan.mode = mode;
    plan.capacity = capacity;
    plan.rows = rows;
    plan.cols = cols;
    plan.soldCount = soldCount;
    plan.occupiedSeats = occupiedSeats;
    return plan;
}

bool SeatingPlan::containsSeat(const Seat& seat) const {
    for (const Seat& current : this->occupiedSeats) {
        if (current == seat) {
            return true;
        }
    }

    return false;
}

SeatingMode SeatingPlan::getMode() const {
    return this->mode;
}

int SeatingPlan::getCapacity() const {
    return this->capacity;
}

int SeatingPlan::getRows() const {
    return this->rows;
}

int SeatingPlan::getCols() const {
    return this->cols;
}

int SeatingPlan::getSoldCount() const {
    return this->soldCount;
}

int SeatingPlan::getAvailableCount() const {
    return this->capacity - this->soldCount;
}

const std::vector<Seat>& SeatingPlan::getOccupiedSeats() const {
    return this->occupiedSeats;
}

bool SeatingPlan::isValidSeat(const Seat& seat) const {
    return this->mode == SeatingMode::AssignedSeats && seat.first >= 1 && seat.first <= this->rows && seat.second >= 1 && seat.second <= this->cols;
}

bool SeatingPlan::isFreeSeat(const Seat& seat) const {
    return this->isValidSeat(seat) && !this->containsSeat(seat);
}

bool SeatingPlan::canReserveGeneral(int count) const {
    return this->mode == SeatingMode::GeneralAdmission && count > 0 && this->soldCount + count <= this->capacity;
}

bool SeatingPlan::canReserveSeats(const std::vector<Seat>& seats) const {
    if (this->mode != SeatingMode::AssignedSeats || seats.empty()) {
        return false;
    }

    std::set<Seat> uniqueSeats;

    for (const Seat& seat : seats) {
        if (!this->isFreeSeat(seat)) {
            return false;
        }

        if (uniqueSeats.contains(seat)) {
            return false;
        }

        uniqueSeats.insert(seat);
    }

    return this->soldCount + (int)seats.size() <= this->capacity;
}

bool SeatingPlan::reserveGeneral(int count) {
    if (!this->canReserveGeneral(count)) {
        return false;
    }

    this->soldCount += count;
    return true;
}

bool SeatingPlan::reserveSeats(const std::vector<Seat>& seats) {
    if (!this->canReserveSeats(seats)) {
        return false;
    }

    for (const Seat& seat : seats) {
        this->occupiedSeats.push_back(seat);
    }

    this->soldCount += (int)seats.size();
    return true;
}

void SeatingPlan::print() const {
    if (this->mode == SeatingMode::GeneralAdmission) {
        std::cout << "Capacity: " << this->capacity << std::endl;
        std::cout << "Sold: " << this->soldCount << std::endl;
        std::cout << "Free: " << this->getAvailableCount() << std::endl;
        return;
    }

    std::cout << "Seating map ([ ] free, [X] occupied):" << std::endl;
    std::cout << "    ";

    for (int c = 1; c <= this->cols; c++) {
        std::cout << c << "   ";
    }

    std::cout << std::endl;

    for (int r = 1; r <= this->rows; r++) {
        std::cout << r << "  ";

        for (int c = 1; c <= this->cols; c++) {
            if (this->containsSeat({ r, c })) {
                std::cout << "[X] ";
            } else {
                std::cout << "[ ] ";
            }
        }

        std::cout << std::endl;
    }
}

std::string SeatingPlan::encodeSeats(const std::vector<Seat>& seats) {
    std::vector<std::string> parts;

    for (const Seat& seat : seats) {
        parts.push_back(std::to_string(seat.first) + "," + std::to_string(seat.second));
    }

    return StringUtils::joinEscaped(parts, ';');
}

std::vector<Seat> SeatingPlan::decodeSeats(const std::string& text) {
    std::vector<Seat> seats;

    if (text.empty()) {
        return seats;
    }

    std::vector<std::string> parts = StringUtils::splitEscaped(text, ';');

    for (const std::string& part : parts) {
        std::vector<std::string> coords = StringUtils::splitEscaped(part, ',');

        if (coords.size() != 2) {
            continue;
        }

        std::optional<int> row = StringUtils::toInt(coords[0]);
        std::optional<int> col = StringUtils::toInt(coords[1]);

        if (row.has_value() && col.has_value()) {
            seats.push_back({ row.value(), col.value() });
        }
    }

    return seats;
}
