CC   =gcc
C_FLAGS=-I../../inc/ -Wall 
BIN  =../../bin
%:%.c
	$(CC) $(C_FLAGS) -o $(BIN)/$@ $^ 

all: cli

clean:
	rm -f $(BIN)/*

.PHONY: all clean cleanall