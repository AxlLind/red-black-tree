TARGETS   := fuzz example
SRC_DIR   := src
BUILD_DIR := build

CC            := gcc
CPPFLAGS      := -I$(SRC_DIR)
CFLAGS        := -g -Wall -Wextra -Wpedantic -Wshadow -Werror -O3
VALGRINDFLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes

.PRECIOUS: $(BUILD_DIR)/%.o
.PHONY: $(TARGETS) $(TARGETS:%=%-valgrind) clean

$(TARGETS): % : $(BUILD_DIR)/%
	./$(BUILD_DIR)/$@

$(TARGETS:%=%-valgrind): %-valgrind : $(BUILD_DIR)/%
	valgrind $(VALGRINDFLAGS) ./$(BUILD_DIR)/$*

$(BUILD_DIR)/%: $(BUILD_DIR)/rb-tree.o $(BUILD_DIR)/%.o
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rfv $(BUILD_DIR)
