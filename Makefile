EXEC = exec

SRC = src/simple.c

CC = clang
CFLAGS = -Wall -Wextra
# CFLAGS += -Werror

LDLIBS = -lSDL2 -lSDL2_image

run: $(EXEC)
	./$(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS)  -o $(EXEC) $(SRC) $(LDLIBS)

clean:
	rm -rf $(EXEC)

re: clean $(EXEC)

fmt:
	clang-format -i $(shell find src -name '*.c')

.PHONY: run clean re fmt
