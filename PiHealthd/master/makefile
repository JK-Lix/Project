objects = new_sever.o common.o 
CFLAGS = -lpthread -lz -lmysqlclient

.PHONY:all
	all:pihealthd

pihealthd : $(objects)
	gcc $(objects) -o pihealthd $(CFLAGS) 

new_sever.o : new_sever.c 
	gcc -c new_sever.c $(CFLAGS)

common.o : ./common/common.c 
	gcc -c ./common/common.c $(CFLAGS)

headers :
	gcc -M new_sever.c  > headers

include headers

.PHONY : install
install: pihealthd
	sudo cp pihealthd /usr/local/bin/pihealthd


.PHONY : clean
clean:
	rm pihealthd *.o headers
