CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -pedantic -Iinclude

SRC = src/main.cpp \
      src/Admin.cpp \
      src/AppState.cpp \
      src/Client.cpp \
      src/CommandParser.cpp \
      src/DateUtils.cpp \
      src/Enums.cpp \
      src/Event.cpp \
      src/EventPrinter.cpp \
      src/EventySystem.cpp \
      src/IdGenerator.cpp \
      src/Notification.cpp \
      src/Request.cpp \
      src/SeatingPlan.cpp \
      src/StorageService.cpp \
      src/StringUtils.cpp \
      src/Ticket.cpp \
      src/TicketedEvent.cpp \
      src/VolunteerEvent.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = eventy

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) eventy.exe $(OBJ)
