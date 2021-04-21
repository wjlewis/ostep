FLAGS=-Wall -Werror

first-fit: first-fit.c
	$(CC) $(FLAGS) -c first-fit.c

test-first-fit: test-first-fit.c
	$(CC) $(FLAGS) test-first-fit.c first-fit.o ../../testing/ctest.o

clean:
	rm -f *.o

.PHONY: clean
