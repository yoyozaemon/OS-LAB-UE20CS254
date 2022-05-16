#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static inline void syserr_exit(const char *fmt, ...)
{
    va_list args;
    int errnum = errno;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    if (errnum != 0)
        fprintf(stderr, "%d: %s\n", errnum, strerror(errnum));
    exit(EXIT_FAILURE);
}

int main(void)
{
    int fd1[2], fd2[2];
    int pid1, pid2;

    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        syserr_exit("Failed to create two pipes: ");
    else if ((pid1 = fork()) < 0)
        syserr_exit("Failed to fork() child 1: ");
    else if (pid1 == 0)
    {
        /* Child 1*/
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        char cad1[100];
        printf("String 1: \n");
        fflush(stdout);
        if (fgets(cad1, sizeof(cad1), stdin) == NULL)
            syserr_exit("child 1 - failed to read from standard input: ");
        cad1[strcspn(cad1, "\r\n")] = '\0';
        write(fd1[1], cad1, strlen(cad1));
        /* Should error check the write! */
        printf("child 1 wrote [%s] to the parent process\n", cad1);
        close(fd1[1]);
        exit(0);
    }
    else if ((pid2 = fork()) < 0)
        syserr_exit("Failed to fork child 2: ");
    else if (pid2 == 0)
    {
        /* Child 1*/
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        printf("String 2: \n");
        fflush(stdout);
        char cad2[100];
        if (fgets(cad2, sizeof(cad2), stdin) == NULL)
            syserr_exit("child 2 - failed to read from standard input: ");
        cad2[strcspn(cad2, "\r\n")] = '\0';
        write(fd2[1], cad2, strlen(cad2));
        /* Should error check the write! */
        printf("child 2 wrote [%s] to the parent process\n", cad2);
        close(fd2[1]);
        exit(0);
    }
    else
    {
        /* Parent */
        char buffer1[100];
        char buffer2[100];
        close(fd2[1]);
        close(fd1[1]);
        ssize_t sz1 = read(fd1[0], buffer1, sizeof(buffer1));
        buffer1[sz1] = '\0';
        close(fd1[0]);
        ssize_t sz2 = read(fd2[0], buffer2, sizeof(buffer2));
        buffer2[sz2] = '\0';
        close(fd2[0]);

        size_t tlen = sz1 + sz2 + sizeof("[]+[]");
        char concat[tlen];
        snprintf(concat, sizeof(concat), "[%s]+[%s]", buffer1, buffer2);

        /* wait for both children */
        int s1, s2;
        int c1 = wait(&s1);
        int c2 = wait(&s2);

        printf("The one child (%d) exited with status 0x%.4X\n", c1, s1);
        printf("T'other child (%d) exited with status 0x%.4X\n", c2, s2);
        printf("Received from %d (%zu bytes) [[%s]]\n", pid1, sz1, buffer1);
        printf("Received from %d (%zu bytes) [[%s]]\n", pid2, sz2, buffer2);
        printf("Concatenated data: <<%s>>\n", concat);
    }
    return 0;
}