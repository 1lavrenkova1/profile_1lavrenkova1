all: main clean

main: main.o cli.o functions.o BMPwork.o
	gcc main.o cli.o functions.o BMPwork.o

main.o: main.c
	gcc -c main.c

functions.o: functions.c
	gcc -c functions.c

cli.o: cli.c
	gcc -c cli.c

BMPwork.o: BMPwork.c
	gcc -c BMPwork.c

clean:
	rm -rf *.o