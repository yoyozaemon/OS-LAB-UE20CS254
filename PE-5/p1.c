#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
int main()
{
    struct stat info;
    char fileName[200];
    printf("Enter filename : ");
    scanf("%s",fileName);
    char cmd[32];
    int ret = 0;
    char date[100];
    printf("Enter date : ");
    scanf("%s",date);
    char *perms;
    printf("Enter the permissions (666) : ");
    scanf("%s",perms);
    sprintf(cmd, "chmod %s %s",perms, fileName);
    stat(fileName,&info);
    ret = system(cmd);
    if (ret == 0 && date < ctime(&info.st_ctime))
        printf("Permissions of file changed successfully\n");
    else
        printf("Unable to change the permissions of file\n");
    return 0;
}
