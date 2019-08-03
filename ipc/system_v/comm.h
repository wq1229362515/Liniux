#pragma once 


#include<stdio.h>
#include<sys/shm.h>
#include <sys/ipc.h>

#define PATHNAME "."
#define PROJ_ID 0x6666

int createshm(int size);
int destroyshm(int shmid);
int getshm(int size);
