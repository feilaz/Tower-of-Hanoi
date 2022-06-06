headers = 

sources = testlib.c

executable = testlib

$(executable): $(headers) $(sources)
	gcc -g -Wall -pedantic $(sources) -o $(executable)

.PHONY: clean
clean:
	rm $(executable)

.PHONY: check
check: $(executable)
	valgrind --leak-check=full --track-origins=yes ./$(executable)