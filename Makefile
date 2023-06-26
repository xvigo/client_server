CC = g++
CFLAGS = -Wall

client: client.cpp
	$(CC) $(CFLAGS) $^ -o $@

run: client
	./client