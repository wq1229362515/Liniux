#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<termios.h>
#include<string.h>




int main(int argc, const char *argv[])
{
	
	struct termios options;
	int fd = open("/dev/ttySAC0",O_RDWR);
	//int fd = open("/dev/ttyUSB0",O_RDWR);
	if(fd == -1){
		perror("open uart file error:");
		return -1;
	}
	else {
		printf("open success!\n");	
	}
	
	
	tcgetattr(fd,&options);

	cfsetispeed(&options,B2400);

	options.c_cflag |= CLOCAL;
	options.c_cflag |= CREAD;
	
	options.c_cflag &= ~CRTSCTS;

	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	options.c_cflag &= ~PARENB;
	options.c_iflag &= ~INPCK;

	options.c_cflag &= ~CSTOPB;
	options.c_oflag &= ~OPOST;
	
	options.c_cflag &= ~ICANON;
	options.c_cflag &= ~ECHO;
	options.c_cflag &= ~ISIG;

	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 1;

	tcsetattr(fd,TCSANOW,&options);
	tcflush(fd,TCIOFLUSH);
	

	char buf[1024];
	char sendstr[] = "hello\n";
	while(1){
		//write 
		write(fd,sendstr,sizeof(sendstr));
		bzero(buf,sizeof(buf));
		sleep(1);
		//read
		int ret = read(fd,buf,sizeof(sendstr));
		printf("ret = %d,buf = %s\n",ret,buf);
		sleep(1);
	}
	close(fd);
	return 0;
}
