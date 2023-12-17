all: mem_test

mem_test: my_malloc.c mem_test.c
	gcc -g -Wall -Wextra my_malloc.c mem_test.c -o mem_test

clean:
	rm -f mem_test