# This code was generated with AI assistance

CXX ?= g++

TARGET := eventy
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

BASE_CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic
CPPFLAGS ?=
CPPFLAGS += -I$(INC_DIR)
CXXFLAGS ?= $(BASE_CXXFLAGS)
LDFLAGS ?=
LDLIBS ?=

SOURCES := $(sort $(wildcard $(SRC_DIR)/*.cpp))
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

.PHONY: all run clean rebuild debug release sanitize help

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET) $(TARGET).exe

rebuild: clean all

debug:
	$(MAKE) clean
	$(MAKE) CXXFLAGS="$(BASE_CXXFLAGS) -O0 -g3" all

release:
	$(MAKE) clean
	$(MAKE) CXXFLAGS="$(BASE_CXXFLAGS) -O2 -DNDEBUG" all

sanitize:
	$(MAKE) clean
	$(MAKE) \
		CXXFLAGS="$(BASE_CXXFLAGS) -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer" \
		LDFLAGS="$(LDFLAGS) -fsanitize=address,undefined" \
		all

help:
	@printf '%s\n' \
		'make           Build the application' \
		'make run       Build and run the application' \
		'make debug     Rebuild with debug symbols' \
		'make release   Rebuild with optimizations' \
		'make sanitize  Rebuild with AddressSanitizer and UBSan' \
		'make clean     Remove generated build files' \
		'make rebuild   Clean and build again'

-include $(DEPENDENCIES)
