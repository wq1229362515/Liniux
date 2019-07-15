#ifndef __RS232_H_
#define __RS232_H_ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

//´®¿Ú½á¹¹
typedef struct _tag_rs232_attr {
	char  prompt;		//prompt after reciving data
	int   baudrate;		//baudrate
	char  databit;		//data bits, 5, 6, 7, 8
	char  debug;		//debug mode, 0: none, 1: debug
	char  echo;			//echo mode, 0: none, 1: echo
	char  fctl;			//flow control, 0: none, 1: hardware, 2: software
	char  parity;		//parity 0: none, 1: odd, 2: even
	char  stopbit;		//stop bits, 1, 2
	int   reserved;		//reserved, must be zero
} tag_rs232_attr;


struct _tag_rs232;
typedef int (*rs232_recv_callback_t)(struct _tag_rs232* m_this, void *data);


//
typedef struct _tag_rs232 {
	int fd;
	pthread_t th;
	int thread_running;
	int * status;
	tag_rs232_attr attr;
} tag_rs232;

// 
#define RS232_RECV_CMD_HEAD  0x79

typedef struct _tag_r_cmd {
	unsigned char head;     //0x79
	unsigned char len_l;	//high length of data
	unsigned char len_h;	//low length of data
	unsigned char ctrl;
	unsigned char cmd;
	unsigned char data_ctrl0;
	unsigned char data_ctrl1;
	unsigned char data_ctrl2;
	unsigned char dst_addr5;
	unsigned char dst_addr4;
	unsigned char dst_addr3;
	unsigned char dst_addr2;
	unsigned char dst_addr1;
	unsigned char dst_addr0;
	unsigned char usr_data_len_l;
	unsigned char usr_data_len_h;
	unsigned char usr_data;
	unsigned char csum;		//l, ctrl, cmd, data
	unsigned char cxor;		//l, ctrl, cmd, data
}tag_r_cmd;


extern int rs232_init(const char *m_devname, 
							  int m_baud, 
							  int m_databit, 
							  int m_parity, 
							  int m_stopbit,
							  int * status);

extern void destroy_rs232(tag_rs232 *this);


#endif
