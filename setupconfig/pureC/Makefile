COMPILER=cc

FLAGS=-g -std=c89
FLAGS+=-Wall -Wextra -Wpedantic

test: main.o functions.o functions_electr.o commonxml.o xmlparsing.o xmlparsing_electr.o
	$(COMPILER) $(FLAGS) $^ -o $@

main.o: main.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

functions.o: functions.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

functions_electr.o: functions_electr.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

commonxml.o: commonxml.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

xmlparsing.o: xmlparsing.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

xmlparsing_electr.o: xmlparsing_electr.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

.PHONY: clean

clean:
	rm -fv *.o test
