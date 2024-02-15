#include <kernel/types.h>
#include <user/user.h>


int MAX_NUM = 36;

void
sieve(int l)
{
    int pl[2] = {-1, -1};
    int p, n;
    
    if((read(l, &p, 4)) == 0) {
        exit(1);
    }

    printf("prime %d\n", p);

    while((read(l, &n, 4)) > 0) {
        if(n % p == 0) { continue; }
        if(pl[0] < 0) {
            pipe(pl);
            if(fork() == 0) {
                close(pl[1]);
                sieve(pl[0]);
                close(pl[0]);
                exit(0);
            } else {
                close(pl[0]);
            }
        }
        write(pl[1], &n, 4);
    }
    close(pl[1]);
    wait(0);
    exit(0);
}

int
main()
{
    int pl[2];
    close(0);
    close(2);

    pipe(pl);
    if(fork() == 0) {
        close(pl[1]);
        sieve(pl[0]);
        close(pl[0]);
        exit(0);
    } else {
        close(pl[0]);
        for(int n = 2; n < MAX_NUM; ++n) {
            write(pl[1], &n, 4);
        }
        close(pl[1]);
    }
    wait(0);
    exit(0);
}