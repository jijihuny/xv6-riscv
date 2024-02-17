#include <kernel/types.h>
#include <kernel/fcntl.h>
#include <user/user.h>
#include <kernel/param.h>


int
main(int argc, char *argv[])
{
    char *xargv[MAXARG];
    char buf[512], *start, *end;
    char **p = argv;
    int i = 0;
    p++;
    while(*p) {
        xargv[i] = *p;
        p++; i++;
    }
    start = buf;
    end = buf;
    while(read(0, end, 512) > 0) {
        end = buf + strlen(buf);
    }

    while((end = strchr(start, '\n'))) {
        *end = 0;
        if(fork() == 0) {
            xargv[argc - 1] = start;
            xargv[argc] = 0;
            exec(xargv[0], xargv);
            exit(0);
        } else {
            wait(0);
        }
        start = end + 1;
    }
    exit(0);
}