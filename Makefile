CC = gcc 
CFLAGS = -Wall -Wextra -Werror
SRC = my_mastermind.c
TARGET = my_mastermind

$(TARGET): $(SRC)
	@$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

.PHONY: fclean

fclean:
	@rm -rf $(TARGET)

run:
	./$(TARGET)

make re:
	@rm -rf $(TARGET)
	@$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
