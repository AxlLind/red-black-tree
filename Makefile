SRC_DIR   := src
BUILD_DIR := build

CC            := gcc
CPPFLAGS      := -I$(SRC_DIR)
CFLAGS        := -g -Wall -Wextra -Wpedantic -Wshadow -Werror -O3
VALGRINDFLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes

.PHONY: fuzz clean

fuzz: $(BUILD_DIR)/fuzz
	./$(BUILD_DIR)/fuzz

fuzz-valgrind: $(BUILD_DIR)/fuzz
	 valgrind $(VALGRINDFLAGS) ./$(BUILD_DIR)/fuzz 10000

$(BUILD_DIR)/fuzz: $(BUILD_DIR)/rb-tree.o $(BUILD_DIR)/fuzz.o
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rfv $(BUILD_DIR)
