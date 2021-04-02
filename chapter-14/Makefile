null: null.c
	$(CC) -std=c99 -Werror -Wall -g -o null null.c

no-free: no-free.c
	$(CC) -std=c99 -Werror -Wall -g -o no-free no-free.c

data: data.c
	$(CC) -std=c99 -Werror -Wall -g -o overflow overflow.c

vector: vector.c
	$(CC) -std=c99 -Werror -Wall -g -c vector.c

vector_test: vector_test.c vector.o
	$(CC) -std=c99 -Werror -Wall -g -o vector_test vector.o vector_test.c
