.PHONY:all
all:mysignal mytest

mytest:mytest.cpp
	g++ -o $@ $^ -std=c++11

mysignal:mysignal.cpp
	g++ -o $@ $^ -std=c++11 -g

.PHONY:clean
clean:
	rm -f mytest mysignal
