# # _:
# # 	gcc -o scheduler scheduler.c -fsanitize=address

_:
	gcc -Wall -o scheduler src/scheduler.c src/process.c src/linkedlist.c src/helper.c src/details.c -lm 

clean:
		rm -f $(OBJ) *.o
