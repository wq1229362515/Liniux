#include<stdio.h>
#include<string.h>
int main(){
    char ch[10] = "\0";
	int i;
    for(i = 0;i<10;i++){
        ch[i] = '-';
		printf("\033[40;34m%-50s\033[0m [\033[40;45m%d%\033[0m]\r",ch,(i+1)*10);
		fflush(stdout);
		sleep(1);
    }
	printf("\n");


    return 0;
}

