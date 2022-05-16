#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1
int write_msg[BUFFER_SIZE] = {1, 4, 2, 4, 6, 8, 0, 9};
int main()
{
int fd[2];
pid_t pid;
if (pipe(fd) == -1) {
printf("Pipe creation failed\n");
return 1;
}
pid = fork();
if (pid == 0) {
close(fd[READ_END]);
close(fd[WRITE_END]);
wait(NULL);
printf("\nPrinting array in parent: ");
for(int i = 0; i < 8; i++)
printf("%d ", write_msg[i]);
printf("\n");
}
else if (pid > 0) {
close(fd[WRITE_END]);
int i;
for (i = 0; i < 8 ; i++) {
for(int j = i + 1 ;j < 8 ; j++) {
if(write_msg[i] > write_msg[j]) {
int temp = write_msg[i];
write_msg[i] = write_msg[j];
write_msg[j] = temp;
}
}
}
printf("Sorted array in child: ");
for(i = 0; i < 8; i++) {
printf("%d ", write_msg[i]);
}
printf("\n");
close(fd[READ_END]);
}
else {
printf("Fork failed\n");
return 0;
}
}
