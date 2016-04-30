all : 
	@make -C client/
	@make -C driver/

clean : 
	@make -C client/ clean 
	@make -C driver/ clean

.PHONY: all clean 

