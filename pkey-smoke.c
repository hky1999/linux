#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

static void fault(int sig, siginfo_t *info, void *unused)
{
    (void)sig; (void)unused;
    _exit(info->si_code==SEGV_PKUERR ? 42 : 43);
}

int main(int argc, char **argv)
{
    (void)argv;
    int code=0;
    int key=syscall(SYS_pkey_alloc,0,0);
    if (key<0) { printf("PKEY_SMOKE allocation_unavailable errno=%d\n",errno); code=1; goto done; }
    volatile unsigned char *p=mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    if (p==MAP_FAILED || syscall(SYS_pkey_mprotect,(void *)p,4096,PROT_READ|PROT_WRITE,key)) {
        printf("PKEY_SMOKE setup_failed errno=%d\n",errno); code=1; goto done;
    }
    *p=0x5a;
    for (int mode=0;mode<3;mode++) {
    pid_t child=fork();
    if (child==0) {
        struct sigaction action={.sa_sigaction=fault,.sa_flags=SA_SIGINFO};
        sigemptyset(&action.sa_mask); sigaction(SIGSEGV,&action,NULL);
        unsigned int pkru,unused;
        __asm__ volatile("rdpkru":"=a"(pkru),"=d"(unused):"c"(0));
        pkru|=1u<<(2*key);
        __asm__ volatile("wrpkru"::"a"(pkru),"c"(0),"d"(0):"memory");
        if (mode==1) syscall(SYS_getpid);
        if (mode==2) syscall(SYS_sched_yield);
        unsigned char value=*p;
        _exit(value==0x5a ? 41 : 44);
    }
    int status;
    if (child<0 || waitpid(child,&status,0)<0) { printf("PKEY_SMOKE fork_failed errno=%d\n",errno); code=1; goto done; }
    printf("PKEY_SMOKE key=%d mode=%s expected=SEGV_PKUERR observed=%s child_status=%d\n",key,
           mode==0?"direct":mode==1?"after_getpid":"after_sched_yield",
           WIFEXITED(status) && WEXITSTATUS(status)==42 ? "permission_fault" :
           WIFEXITED(status) && WEXITSTATUS(status)==41 ? "read_succeeded" : "other",status);
    if (!WIFEXITED(status) || WEXITSTATUS(status)!=42) code=1;
    }
done:
    fflush(stdout);
    if (argc>1) return code;
    for (;;) pause();
}
