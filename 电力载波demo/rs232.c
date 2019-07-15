#include "rs232.h"

static void *rs232_r_thread(void *arg)
{
	//BUG: wait thread running;
	//DO NOT CARE
	tag_rs232 *m_this = (tag_rs232*)arg;
	int fd = m_this->fd;
	unsigned char r_buf[128];
	int m_remain = 0;
	int m_rlen   = 0;
	unsigned char *m_pbuf;
	int m_move;
	unsigned char *m_p;

	memset(r_buf, 0, sizeof(r_buf));

	while (m_this->thread_running) {
		while (m_remain < sizeof(tag_r_cmd)) {
			m_rlen = read(fd, (void *)&r_buf[m_remain], sizeof(tag_r_cmd) - m_remain);
			if (m_rlen == -1) {
				fprintf(stderr, "read rs232 thread() err %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			m_remain += m_rlen;
			printf("hello m_remain = %d  m_rlen = %d\n", m_remain, m_rlen);
		}

		m_p = r_buf;
		if (*m_p != RS232_RECV_CMD_HEAD) {
			m_p++;
			m_remain--;
			if (m_remain == 0)
				continue;
		}

		if (m_remain < sizeof(tag_r_cmd)) {
			memmove(r_buf, &r_buf[sizeof(tag_r_cmd)-m_remain], -m_remain);				
			continue;	
		}
	
		if (m_this->status) {
			write(fd, r_buf, sizeof(tag_r_cmd));
			*m_this->status = ((tag_r_cmd *)r_buf)->usr_data;
			printf("data is %x\n", *m_this->status);
			m_remain = 0;
		}
	}

	return NULL;
}


static int convbaud(unsigned long int baudrate)
{
    switch (baudrate) {
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        default:
            return B9600;
    }
}

static int rs232_set_attr(int fd, tag_rs232_attr* m_pattr)
{
    struct  termios termios_old, termios_new;
    int     baudrate, tmp;
    char    databit, stopbit, parity, fctl;

    bzero(&termios_old, sizeof(termios_old));
    bzero(&termios_new, sizeof(termios_new));

	cfmakeraw (&termios_new);
    tcgetattr(fd, &termios_old);         	//
    /*------------设置端口属性----------------*/
    //baudrates
    baudrate = convbaud(m_pattr->baudrate);
    
	cfsetispeed(&termios_new, baudrate);        //填入串口输入端的波特率
    cfsetospeed(&termios_new, baudrate);        //填入串口输出端的波特率
   
	termios_new.c_cflag |= CLOCAL;          //控制模式，保证程序不会成为端口的占有者
    termios_new.c_cflag |= CREAD;           //控制模式，使能端口读取输入的数据

    // 控制模式，flow control
    fctl = m_pattr->fctl;
    switch(fctl) {
	case 0:
	case '0':
		termios_new.c_cflag &= ~CRTSCTS;        //no flow control
		break;
	
	case 1:
	case '1':
        termios_new.c_cflag |= CRTSCTS;         //hardware flow control
        break;
	
	case 2:
	case '2':
        termios_new.c_iflag |= IXON | IXOFF |IXANY; //software flow control
        break;
    }

    //控制模式，data bits
    termios_new.c_cflag &= ~CSIZE;      //控制模式，屏蔽字符大小位
    databit = m_pattr->databit;
    switch(databit) {
   	case 5:     
	case '5':
        termios_new.c_cflag |= CS5;
   	
	case 6:
	case '6':
        termios_new.c_cflag |= CS6;
   	
	case 7:
	case '7':
        termios_new.c_cflag |= CS7;
      
	default:
		termios_new.c_cflag |= CS8;
	}

    //控制模式 parity check
    parity = m_pattr->parity;
	switch (parity) {
	case  0:
	case 'n':
	case 'N':
		termios_new.c_cflag &= ~PARENB;     //no parity check
		break;

	case 'O':	
	case 'o':
	case '1':
		termios_new.c_cflag |= PARENB;      //odd check
		termios_new.c_cflag &= ~PARODD;
		break;

	case '2':
	case 'E':
	case 'e':
		termios_new.c_cflag |= PARENB;      //even check
		termios_new.c_cflag |= PARODD;
		break;
	}

    //控制模式，stop bits
    stopbit = m_pattr->stopbit;
    if(stopbit == '2' || stopbit == 2)
        termios_new.c_cflag |= CSTOPB;  //2 stop bits
    else
        termios_new.c_cflag &= ~CSTOPB; //1 stop bits

    termios_new.c_oflag &= ~OPOST;          //输出模式，原始数据输出
    termios_new.c_cc[VMIN]  = 1;            //控制字符, 所要读取字符的最小数量
    termios_new.c_cc[VTIME] = 15;            //控制字符, 读取第一个字符的等待时间    unit: (1/10)second

    tcflush(fd, TCIFLUSH);               //溢出的数据可以接收，但不读
    tmp = tcsetattr(fd, TCSANOW, &termios_new);  //设置新属性，TCSANOW：所有改变立即生效    tcgetattr(fdcom, &termios_old);
    
	return (tmp);
}

extern int rs232_init(const char *m_devname, 
							  int m_baud, 
							  int m_databit, 
							  int m_parity, 
							  int m_stopbit,
							  int * status)
{	
	tag_rs232 *m_ptmp;

	if (NULL == m_devname) 
		return -1;

	m_ptmp = malloc(sizeof(tag_rs232));
	if (NULL == m_ptmp)
		return -1;
	memset(m_ptmp, 0, sizeof(tag_rs232));

	m_ptmp->fd = open(m_devname, O_RDWR|O_NOCTTY);
	if (-1 == m_ptmp->fd) {
		fprintf(stderr, "%s:open():%s\n", __func__, strerror(errno));
		free(m_ptmp);
		return -1;
	}
	
	m_ptmp->attr.baudrate = m_baud;
	m_ptmp->attr.databit  = m_databit;
	m_ptmp->attr.parity   = m_parity;
	m_ptmp->attr.stopbit  = m_stopbit;
	
	if (-1 == rs232_set_attr(m_ptmp->fd, &m_ptmp->attr)) {
		fprintf(stderr, "%s:rs232_set_attr():%s\n", __func__, strerror(errno));
		close(m_ptmp->fd);
		free(m_ptmp);
		return -1;
	}
	
	m_ptmp->status = status;
	m_ptmp->thread_running = 1;

	if (-1 == pthread_create(&m_ptmp->th, NULL, rs232_r_thread, m_ptmp)) {
		fprintf(stderr, "%s:pthread_create():%s\n", __func__, strerror(errno));
		close(m_ptmp->fd);
		free(m_ptmp);
		return -1;
	}	
	return 0; 
}

void destroy_rs232(tag_rs232 *m_this)
{
	if (NULL == m_this) 
		return; 
	m_this->thread_running = 0;
	pthread_join(&m_this->th);
	
	close(m_this->fd);
	free(m_this);
}
