#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void clearBuffer(char * buf){
    for(int i = 0;i<25;i++){
        buf[i]= 0;
    }
}

int main(){
    int fd;
    char buf [25];
    char test [25] = "hallo das ist ein Test :)";
    fd = open("sTest",O_RDONLY);
    if(fd < 0){
        return -1;
    }

    lseek(fd,6,SEEK_SET);
    
    if(read(fd,buf,25) >= 0){
        if(strcmp(buf,&test[6]) == 0){
            printf("SEEK_SET: Test successfully ended\n");
        }else{
            printf("SEEK_SET: Test failed: '%s' != '%s'\n",buf,&test[6]);
        }
    }

    lseek(fd,6,SEEK_SET);
    clearBuffer(buf);
    lseek(fd,6,SEEK_CUR);

    if(read(fd,buf,25) >= 0){
        if(strcmp(buf,&test[12]) == 0){
            printf("SEEK_CUR: Test successfully ended\n");
        }else{
            printf("SEEK_CUR: Test failed: '%s' != '%s'\n",buf,&test[12]);
        }
    }

    clearBuffer(buf);
    lseek(fd,-6,SEEK_END);

    if(read(fd,buf,25) >= 0){
        if(strcmp(buf,&test[19]) == 0){
            printf("SEEK_END: Test successfully ended\n");
        }else{
            printf("SEEK_END: Test failed: '%s' != '%s'\n",buf,&test[19]);
        }
    }
   
    exit(0);
}

