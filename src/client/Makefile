CC   =gcc
BIN  =../../bin
%:%.c
	$(CC) -Wall -o $(BIN)/$@ $^ 

all: cli

clean:
	rm -f $(BIN)/*

.PHONY: all clean cleanall