#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
 
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

int main()
{
        int fd;
       int value,option;
        int current_counter;
 
        printf("\nOpening Driver\n");
        fd = open("/dev/timer0_device", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
   printf("\nchoose the option\n1. delay \n 2. current counter value \n");
   scanf("%d",&option);
 switch(option)
  {
   case 1: 
            printf("how much delay do you want in ms ?\n");
            scanf("%d" , &value);
            ioctl(fd, WR_VALUE, (int32_t*) &value);
             break;
  case 2 :  ioctl(fd, RD_VALUE, (int32_t*) &current_counter);
             printf("current_counter value is %d\n", current_counter);
             break;
}
       printf("Closing Driver\n");
        close(fd);
}
