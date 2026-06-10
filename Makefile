CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude

SRC = src/main.cpp \
      src/Client.cpp \
      src/Event.cpp \
      src/EventySystem.cpp

TARGET = eventy

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) eventy.exe
