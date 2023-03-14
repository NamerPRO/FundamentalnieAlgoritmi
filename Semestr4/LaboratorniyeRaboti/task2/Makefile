CC = g++
FLAGS = -g -std=c++2a -pedantic -Wextra -w -pipe -Werror
FILES = main.cpp standard_allocator.cpp memory_allocator.cpp
LOGGER_FILES = ./logger/basics.cpp ./logger/cpp_time.cpp ./logger/logger.cpp ./logger/logger_builder.cpp ./logger/standard_logger_builder.cpp ./logger/standard_logger.cpp

run: $(FILES)
	$(CC) $(FLAGS) $(LOGGER_FILES) $(FILES) -o $@.out
