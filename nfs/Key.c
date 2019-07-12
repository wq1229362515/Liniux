#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
int  main (){
	int fd1;
	int fd2;
	char key1,key2;
	fd1 = open("/dev/s2",O_RDWR);
	if(fd1 == -1){
		perror("open error");
		return -1;
	}
	fd2 = open("/dev/s3",O_RDWR);
	if(fd2 == -1){
		perror("open error");
		return -1;
	}	
	/*
	   while(1){
	   read(fd1,&key,1);
	   printf("-----\n");
	   }
	   */
	int pid = fork();
	if(pid < 0){
		perror("process filed");
		exit(1);
	}
	//son process		
	if(pid  ==  0){
		int flag = 0;
		int fd = open("/dev/myled2",O_RDWR);
		if(fd == -1){
			printf("open error ");
		}
		while(1){
			read(fd1,&key1,1);	
			printf("i am son,use led2 \n");
			if(flag == 0){
				write(fd,"led2on",6);
				flag = 1;
				continue;
			} 
			if(flag == 1){
				write(fd,"led2off",7);
				flag = 0;
				continue;	
			}
		}
	}
	//father process
	if(pid >  0){
		int flag = 0;
		int fd = open("/dev/myled1",O_RDWR);
		if(fd == -1){
			printf("open error");
		}
		while(1){
			read(fd2,&key2,1);	
			printf("i am father,use Led1\n");
			if(flag == 0){
				write(fd,"led1on",6);
				flag = 1;
				continue;
			} 
			if(flag == 1){
				write(fd,"led1off",7);
				flag = 0;
				continue;	
			}
		}
	}
	close(fd1);
	close(fd2);
	return 0;
}
