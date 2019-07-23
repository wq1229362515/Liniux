all:Tcli Tsrv processTsrv threadTsrv
threadTsrv:threadTsrv.cpp
	g++ $^ -o $@ -lpthread
processTsrv:processTsrv.cpp
	g++ $^ -o $@
Tcli:Tcli.cpp
	g++ $^ -o $@
Tsrv:Tsrv.cpp
	g++ $^ -o $@
clientUDP:clientUDP.cpp
	g++ $^ -o $@
serviceUDP:serviceUDP.cpp
	g++ $^ -o $@

