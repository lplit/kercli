.PHONY:all clean
INC = /inc

all:
	@make -I$(INC) -C client/
	@make -I$(INC) -C module/

clean:
	make -C client/ clean
	make -C module/ clean
