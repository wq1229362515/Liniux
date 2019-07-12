#include<stdio.h>
#include<fcntl.h>
#include<error.h>
#include<stdlib.h>
#include<string.h>
int  main (){

	char buf[32] = {0};
	int fduart = open("/dev/ttySAC01",O_RDWR);
	if(fduart == -1){
		perror("can't open uart");
		return -1;
	}
	while(1){
		bzero(buf,sizeof(buf));
		printf("recever  data: ");
		int ret = read(fduart,buf,sizeof(buf));	
		if(ret == -1 ){
			perror("read  error");
		}
		printf("%s\n",buf);
		sleep(1);
	}
	return 0;
}
