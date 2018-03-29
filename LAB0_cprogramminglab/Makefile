CC = clang
CFLAGS = -O0 -g -Wall -Werror


all: qtest
	-tar -cf handin.tar queue.c queue.h Makefile

queue.o: queue.c queue.h harness.h
	$(CC) $(CFLAGS) -c queue.c

#you need to add queue.o to the build process and the linking process here
qtest: qtest.c report.c console.c harness.c queue.o
	$(CC) $(CFLAGS) -o qtest qtest.c report.c console.c harness.c queue.o

test: qtest driver.py
	chmod +x driver.py
	./driver.py

clean:
	rm -f *.o *~ qtest 
	rm -rf *.dSYM
	rm -f *.tar
	(cd traces; rm -f *~)

