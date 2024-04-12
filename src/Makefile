CC     = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Wpedantic -Wconversion \
	 -Wno-sign-conversion -Wdouble-promotion -Wno-unused-parameter \
	 -Wno-unused-function -O0 -g3
LFLAGS = -lSDL2

TRGT = hlf
SRCS = hlf.c
OBJS = $(SRCS:.c=.o)

all: $(TRGT)

$(TRGT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(TRGT)


%.o: %.c
	$(CC) $(CLFAGS) $(LFLAGS) -c $^ -o $@

clean:
	@rm -f $(TRGT) $(OBJS)
	@echo "clean"

re: clean all

run: all
	./$(TRGT)
