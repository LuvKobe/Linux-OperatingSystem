.PHONY:all
all: mybin myproc 

mybin:mybin.c
	g++ -o $@ $^
myproc:test.cpp
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f myproc mybin
