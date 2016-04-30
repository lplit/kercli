.PHONY:all clean


all:
	@make -C src/client/
	@make -C src/module/

clean:
	make -C src/client/ clean
	make -C src/module/ clean
