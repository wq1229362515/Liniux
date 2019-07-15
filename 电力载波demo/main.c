#include "include.h"

#define LED1ON		0x11
#define LED2ON		0x21
#define LED3ON		0x31
#define LED4ON		0x41
#define LEDALLON	0xAA
#define LED1OFF 	0x10
#define LED2OFF 	0x20
#define LED3OFF 	0x30
#define LED4OFF 	0x40
#define LEDALLOFF 	0x55
int fd[4] = {0};

int led_init()
{
	fd[0] = open("/dev/myled1", O_RDWR);
	fd[1] = open("/dev/myled2", O_RDWR);
	fd[2] = open("/dev/myled3", O_RDWR);
	fd[3] = open("/dev/myled4", O_RDWR);
	if((fd[0] < 0) || (fd[1] < 0) || (fd[2] < 0) || (fd[3] < 0))
	{
		return -1;
	}

	return 0;
}

void led_ctrl(int led)
{
	switch(led)
	{
		case LED1ON :
			printf("led on!\n");
			write(fd[0], "led1on", 6);
			break;
		case LED2ON :
			write(fd[1], "led2on", 6);
			break;
		case LED3ON :
			write(fd[2], "led3on", 6);
			break;
		case LED4ON :
			write(fd[3], "led4on", 6);
			break;
		case LEDALLON :
			write(fd[0], "led1on", 6);
			write(fd[1], "led2on", 6);
			write(fd[2], "led3on", 6);
			write(fd[3], "led4on", 6);
			break;
		case LED1OFF :
			printf("led on!\n");
			write(fd[0], "led1off", 7);
			break;
		case LED2OFF :
			write(fd[1], "led2off", 7);
			break;
		case LED3OFF :
			write(fd[2], "led3off", 7);
			break;
		case LED4OFF :
			write(fd[3], "led4off", 7);
			break;
		case LEDALLOFF :
			write(fd[0], "led1off", 7);
			write(fd[1], "led2off", 7);
			write(fd[2], "led3off", 7);
			write(fd[3], "led4off", 7);
			break;
		default:
			break;
	}

	return;
}

int main(int argc, const char *argv[])
{
	int led = -1;
	int led_cmd = -1;
	int led_status = -1;
	
	int ret = rs232_init("/dev/ttySAC0", 9600, 8, 'N', 1, &led_cmd);
	if (-1 == ret) {
		printf("rs232_init error!!!\n");
		return 1;
	}

	led = led_init();
	if(-1 == led)
	{
		printf("led_init error !!!\n");
		return 1;
	}

	while (1) {
		usleep(50 * 1000);
		if(led_cmd != led_status)
		{
			led_status = led_cmd;
			led_ctrl(led_cmd);
		}
	}

	return 0;
}

