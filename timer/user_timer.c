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
        unsigned long current_counter;
 
        printf("\nOpening Driver\n");
        fd = open("/dev/timer0_device", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }

     ioctl(fd, RD_VALUE, (int32_t*) &current_counter);
        printf("current_counter value is %ld\n", current_counter);
  
       printf("Closing Driver\n");
        close(fd);
}
