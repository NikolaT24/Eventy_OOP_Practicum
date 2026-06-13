#pragma once

#include <expected>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include "Enums.h"

using Seat = std::pair<int, int>;

class SeatingPlan {
private:
    SeatingMode mode;
    int capacity;
    int rows;
    int columns;
    int soldCount;
    std::vector<Seat> occupiedSeats;
    bool contains(const Seat& seat) const;

public:
    static SeatingPlan generalAdmission(int capacity);
    static SeatingPlan assignedSeats(int rows, int columns);
    static SeatingPlan restore(SeatingMode mode, int capacity, int rows, int columns, int soldCount, std::vector<Seat> occupiedSeats);

    SeatingMode getMode() const;
    int getCapacity() const;
    int getRows() const;
    int getColumns() const;
    int getSoldCount() const;
    int getAvailableCount() const;
    const std::vector<Seat>& getOccupiedSeats() const;

    bool isValid(const Seat& seat) const;
    bool isFree(const Seat& seat) const;
    bool canReserve(int count) const;
    bool canReserve(const std::vector<Seat>& seats) const;

    void reserve(int count);
    void reserve(const std::vector<Seat>& seats);
    void print(std::ostream& output) const;

    static std::expected<Seat, std::string> parseSeat(const std::string& value);
    static std::string seatToString(const Seat& seat);
    static std::string encodeSeats(const std::vector<Seat>& seats);
    static std::vector<Seat> decodeSeats(const std::string& value);

private:
    SeatingPlan(SeatingMode mode, int capacity, int rows, int columns, int soldCount, std::vector<Seat> occupiedSeats);
};
