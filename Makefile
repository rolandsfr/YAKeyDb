objects := main.o core.o parse.o api.o server.o
flags := -Wall -Wextra  -std=c99 

# link (sanitizer flags must be passed at link time too)
main: $(objects)
	gcc $(flags) $(objects) -o main

# compile main
main.o: main.c
	gcc $(flags) -c main.c -o main.o

# compile helpers
core.o: core.c
	gcc $(flags) -c core.c -o core.o 

parse.o: parse.c
	gcc $(flags) -c parse.c -o parse.o 
	
api.o: api.c
	gcc $(flags) -c api.c -o api.o 

server.o: server.c
	gcc $(flags) -c server.c -o server.o 

# debug
debug: flags += -g 
debug: clean main

# clean
clean:
	rm -f main $(objects) core