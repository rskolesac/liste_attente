TARGET = program

CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -g -I./modules/h

SRC_DIR = ./modules/c
INC_DIR = ./modules/h

MODULE_SRCS = $(SRC_DIR)/gestion.c $(SRC_DIR)/client.c $(SRC_DIR)/alea1.c $(SRC_DIR)/alea2.c
MAIN_SRC = main.c
SRCS = $(MAIN_SRC) $(MODULE_SRCS)

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)  # <--- Tabulation ici

%.o: %.c $(INC_DIR)/struct.h
	$(CC) $(CFLAGS) -c $< -o $@  # <--- Tabulation ici

clean:
	rm -f $(OBJS) $(TARGET)  # <--- Tabulation ici
