cc=g++
LD=-DMYSELF
.PHONY:all
all:calserver calclient

calclient:calClient.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp #${LD}

calserver:calServer.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp ${LD}

.PHONY:clean
clean:
	rm -f calclient calserver
