.PHONY:all clean


all:
	@make -C client/
	@make -C module/

clean:
	make -C client/ clean
	make -C module/ clean
