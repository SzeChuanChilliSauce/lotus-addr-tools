src=$(wildcard *.c)
obj=$(patsubst %.c,%.o,$(src))

all:xxx
	gcc  -otest $(obj)  -L ./ -lfilecoin -lsecp256k1 -lOpenCL -ldl -lrt -lpthread -lgcc_s -lc -lm -lutil 

xxx:
	gcc -c $(src)

clean:
	rm *.o

