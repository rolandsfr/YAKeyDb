objects := main.o api.o cli.o
flags := -w -std=c99 

# link
main: $(objects)
	gcc $(objects) -o main

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