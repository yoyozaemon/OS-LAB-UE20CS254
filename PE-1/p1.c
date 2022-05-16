#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
int main()
{
int fd;
if((fd=fork())<0)
{
printf("error in creating child");
exit(1);
}
if(fd==0)
kill(getpid(),SIGKILL);
else
sleep(2);
system("ps -f");
return 0;
}
