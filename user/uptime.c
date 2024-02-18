#include <kernel/types.h>
#include <user/user.h>

int
main()
{
    printf("%d ticks\n", uptime());
    return 0;
}