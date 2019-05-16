#include<stdio.h>
#include<string.h>
void Test(){
    char string[10] = "\0";
    char *str = string;
    memcpy(str,"hello",5);
    printf("%s\n",str);
}
int main (){
    int i;
    for(i = 0;i<3;i++){
        printf("this is a progam\n");  
    }
    Test();
    return 0;
}
