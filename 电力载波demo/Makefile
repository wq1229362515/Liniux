TARGET:=led_app
CROSS_COMPILE=arm-linux-
CFLAGS=-o
CC=gcc
AR=ar  

SRC=main.c rs232.a
INCLUDE=include.h rs232.h
LDFLAGS=-L./ -lpthread  

$(TARGET):$(SRC)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $@ $^ $(LDFLAGS)

rs232.a:rs232.c rs232.h
	$(CROSS_COMPILE)$(CC) -c -o rs232.o rs232.c
	$(CROSS_COMPILE)$(AR) -crv $@ rs232.o

.PHONY:clean
clean:
	rm $(TARGET) *.gch.* *.o *.a


