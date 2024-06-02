TARGET = robokssay
CFLAGS = -Wall -O2

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(TARGET)
