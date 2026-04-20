CC = gcc
LIBS = -lm

all: modem

wav: modem.c
	$(CC) -o modem modem.c $(LIBS)

clean:
	rm -f modem *.o *.txt
