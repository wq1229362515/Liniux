
hellomk:hellomk.o
	gcc hellomk.o -o hellomk
hellomk.o:hellomk.s
	gcc -c hellomk.s -o hellomk.o
hellomk.s:hellomk.i
	gcc -S hellomk.i -o hellomk.s
hellomk.i:hellomk.c
	gcc -E hellomk.c -o hellomk.i

.PHONY:clean
clean:
	rm -f hellomk.i hellomk.s hellomk.o hellomk

