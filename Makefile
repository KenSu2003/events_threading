CC = gcc
# HEADER = include/header.h
CFLAGS = -I.

# all: main

# main

poul_holm_event_threads: poul_holm_event_threads.o
	$(CC) -o poul_holm_event_threads poul_holm_event_threads.o -l pthread
