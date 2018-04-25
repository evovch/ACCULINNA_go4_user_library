COMPILER=cc

FLAGS=-g -std=c89
FLAGS+=-Wall -Wextra -Wpedantic

test: main.o functions.o xmlparsing.o
	$(COMPILER) $(FLAGS) $^ -o $@

main.o: main.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

functions.o: functions.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

xmlparsing.o: xmlparsing.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

.PHONY: clean

clean:
	rm -fv *.o test
