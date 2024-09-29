exec = Nova
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec) -O2

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	cp ./Nova /usr/local/bin/Nova

clean:
	-rm *.out
	-rm *.o 
	-rm src/*.o