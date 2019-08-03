.PHONY:all
all:client serv
client:client.c comm.c
	gcc -o $@ $^
serv:serv.c comm.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f client serv
