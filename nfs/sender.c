#include<stdio.h>
#include<fcntl.h>
#include<error.h>
#include<stdlib.h>
#include<string.h>

int  main (int argc,const char* argv[]){

	char buf [32] = {"hello"};
	int fduart = open("/dev/ttySAC0",O_RDWR);
	if(fduart == -1){
		perror("can't open uart");
		return -1;
	}
	while(1){
		printf("input data : %s  \n",buf);
		int ret = write(fduart,buf,strlen(buf));	
		if(ret == -1 ){
			perror("write  error");
		}
		printf("input data success! ret = %d \n",ret);
		sleep(1);
	}
	return 0;
}
