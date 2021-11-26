SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SRC))
garbagecollector: $(OBJ)
	gcc -o $@ $(OBJ) -Wall
clean:
	rm *.o garbagecollector
