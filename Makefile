objects := main.o api.o cli.o
flags := -Wall -Wextra -fsanitize=address -std=c99 

# link (sanitizer flags must be passed at link time too)
main: $(objects)
	gcc $(flags) $(objects) -o main

# compile main
main.o: main.c
	gcc $(flags) -c main.c -o main.o

# compile helpers
api.o: api.c
	gcc $(flags) -c api.c -o api.o 

cli.o: cli.c
	gcc $(flags) -c cli.c -o cli.o 

# debug
debug: flags += -g 
debug: clean main

# clean
clean:
	rm -f main main.o api.o cli.o