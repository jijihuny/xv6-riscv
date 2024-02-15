#include <kernel/types.h>
#include <user/user.h>


int
main()
{
    int p1[2], p2[2];
    int parent, child;
    char ping[1], pong[1];
    pipe(p1);
    pipe(p2);

    if(fork() == 0) {
        child = getpid();
        close(p1[0]);
        read(p1[1], ping, 1);
        fprintf(1, "%d: received ping\n", child);
        close(p1[1]);
        close(p2[1]);
        write(p2[0], " ", 1);
        close(p2[0]);
        exit(0);
    } else {
        parent = getpid();
        close(p1[1]);
        write(p1[0], " ", 1);
        close(p1[0]);
        close(p2[0]);
        read(p2[1], pong, 1);
        close(p2[1]);
        wait(0);
        fprintf(1, "%d: received pong\n", parent);
        exit(0);
    }
}