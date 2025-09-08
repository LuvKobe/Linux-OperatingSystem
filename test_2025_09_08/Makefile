cc=g++

.PHONY:all
all:tcpclient tcpserver

tcpclient:tcpClient.cc
	$(cc) -o $@ $^ -std=c++11
tcpserver:tcpServer.cc
	$(cc) -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f tcpclient tcpserver