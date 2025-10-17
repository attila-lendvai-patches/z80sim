CC := gcc
CFLAGS := -Wall -O3
LDFLAGS := -lz80ex
TARGET := z80sim
SRC := z80sim.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
