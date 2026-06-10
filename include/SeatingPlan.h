#ifndef SEATING_PLAN_H
#define SEATING_PLAN_H

#include <string>
#include <vector>
#include <utility>
#include "Enums.h"

using Seat = std::pair<int, int>;

class SeatingPlan {
private:
    SeatingMode mode;
    int capacity;
    int rows;
    int cols;
    int soldCount;
    std::vector<Seat> occupiedSeats;

    bool containsSeat(const Seat& seat) const;

public:
    SeatingPlan();
    static SeatingPlan general(int capacity);
    static SeatingPlan assigned(int rows, int cols);
    static SeatingPlan fromStored(SeatingMode mode, int capacity, int rows, int cols, int soldCount, const std::vector<Seat>& occupiedSeats);

    SeatingMode getMode() const;
    int getCapacity() const;
    int getRows() const;
    int getCols() const;
    int getSoldCount() const;
    int getAvailableCount() const;
    const std::vector<Seat>& getOccupiedSeats() const;

    bool isValidSeat(const Seat& seat) const;
    bool isFreeSeat(const Seat& seat) const;
    bool canReserveGeneral(int count) const;
    bool canReserveSeats(const std::vector<Seat>& seats) const;
    bool reserveGeneral(int count);
    bool reserveSeats(const std::vector<Seat>& seats);
    void print() const;

    static std::string encodeSeats(const std::vector<Seat>& seats);
    static std::vector<Seat> decodeSeats(const std::string& text);
};

#endif
