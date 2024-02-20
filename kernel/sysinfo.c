#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/riscv.h"
#include "kernel/spinlock.h"
#include "kernel/proc.h"
#include "kernel/sysinfo.h"
#include "kernel/defs.h"

uint64
sys_sysinfo()
{
    uint64 addr;
    struct sysinfo info;
    struct proc *p;

    p = myproc();
    argaddr(0, &addr);
    info.freemem = getfreemem();
    info.nproc = getnproc();
    if(copyout(p->pagetable, addr, (char *)&info, sizeof info) < 0) {
        return -1;
    };
    return 0;
}