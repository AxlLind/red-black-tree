TARGETS       := fuzz example
CC            := gcc
CFLAGS        := -I src -g -Wall -Wextra -Wpedantic -Wshadow -Werror -O3
VALGRINDFLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes

.PRECIOUS: build/%.o
.PHONY: $(TARGETS) $(TARGETS:%=%-valgrind) clean

$(TARGETS): % : build/%
	./build/$@

$(TARGETS:%=%-valgrind): %-valgrind : build/%
	valgrind $(VALGRINDFLAGS) ./build/$*

build/%: build/rb-tree.o build/%.o
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p $@

clean:
	rm -rfv build
