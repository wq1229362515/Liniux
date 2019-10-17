.PHONY:all
all:mysql

mysql:mysql.cpp
	g++ $^ -o $@ -L/usr/lib64/mysql -lmysqlclient

.PHONY:clean

clean:
	rm mysql 
