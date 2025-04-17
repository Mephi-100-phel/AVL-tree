all:
	gcc tree.c file_read.c ./dop/dop.c main.c dialog.c graph.c ./readline/readline.c ./getint/insert.c -g -lcgraph -lgvc -o lab4b.out

time:
	gcc tree.c ./timing/timing.c -g -o time.out

clean:
	rm -rf *.o *.a *.out *.txt *.bin

