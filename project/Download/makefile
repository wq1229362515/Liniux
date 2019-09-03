all:http_serv http_client
http_client:http_client.cpp
	g++ -std=c++0x $^ -o $@ -lpthread -lboost_filesystem -lboost_system 
http_serv:http_serv.cpp
	g++ -std=c++0x $^ -o $@ -lpthread -lboost_filesystem -lboost_system 
